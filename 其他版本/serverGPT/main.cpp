#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/epoll.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

const int PORT = 80;
const int NUM_WORKERS = 4;
const int MAX_EVENTS = 1024;
const int NUM_THREADS = 8;

struct Worker {
    int epoll_fd;
    std::queue<int> connection_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
};

// 线程池
class ThreadPool {
public:
    ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; i++) {
            threads.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this]() { return !tasks.empty() || stop; });

                        if (stop) {
                            break;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &thread : threads) {
            thread.join();
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop = false;
};

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }

    return 0;
}

std::string get_file_contents(const std::string &file_path) {
    std::ifstream file_stream(file_path, std::ios::in | std::ios::binary);
    if (!file_stream) {
        return "";
    }

    std::string contents((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
    return contents;
}

void send_response(int fd, int status_code, const std::string &content) {
    std::string status_message;
    if (status_code == 200) {
        status_message = "OK";
    } else if (status_code == 404) {
        status_message = "Not Found";
    } else {
        status_message = "Unknown";
    }

    std::stringstream response;
    response << "HTTP/1.1 " << status_code << " " << status_message << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Content-Type: text/html\r\n";
    response << "\r\n";
    response << content;

    std::string response_str = response.str();
    send(fd, response_str.data(), response_str.size(), 0);
}

std::map<std::string, std::string> parse_http_headers(const std::string &headers_str) {
    std::map<std::string, std::string> headers;
    std::stringstream ss(headers_str);
    std::string line;

    while (std::getline(ss, line)) {
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            headers[key] = value;
        }
    }

    return headers;
}

void handle_io(int fd) {
    std::cout<<"read ac"<<std::endl;
    // 读取请求数据
    char buffer[4096];
    ssize_t bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::string request(buffer);

        // 解析请求行
        std::string::size_type pos = request.find("\r\n");
        std::string request_line = request.substr(0, pos);
        std::stringstream ss(request_line);
        std::string method, uri, version;
        ss >> method >> uri >> version;

        // 解析请求头
        std::string headers_str = request.substr(pos + 2);
        std::map<std::string, std::string> headers = parse_http_headers(headers_str);

        // 输出解析结果
        std::cout << "Method: " << method << std::endl;
        std::cout << "URI: " << uri << std::endl;
        std::cout << "Version: " << version << std::endl;
        std::cout << "Headers:" << std::endl;
        for (const auto &header : headers) {
            std::cout << "  " << header.first << ": " << header.second << std::endl;
        }

        // 根据请求的URI读取文件内容
        std::string file_path = "." + uri; // 假设请求的文件位于当前目录下
        std::string file_content = get_file_contents("/home/jiao/Desktop/doc/html/index.html");

        int status_code;
        if (file_content.empty()) {
            status_code = 404;
            file_content = "<html><body><h1>404 Not Found</h1></body></html>";
        } else {
            status_code = 200;
        }

        // 发送HTTP响应
        send_response(fd, status_code, file_content);

        // 关闭客户端连接
        close(fd);
    }
}

void worker_function(Worker *worker, ThreadPool &thread_pool) {
    epoll_event events[MAX_EVENTS];

    while (true) {
        int num_events = epoll_wait(worker->epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < num_events; i++) {
            int client_fd = events[i].data.fd;
            // 将I/O任务提交给线程池处理
            std::cout<<"提交"<<std::endl;
            thread_pool.enqueue([client_fd]() { handle_io(client_fd); });
        }

        std::unique_lock<std::mutex> lock(worker->queue_mutex);
        while (!worker->connection_queue.empty()) {
            int client_fd = worker->connection_queue.front();
            worker->connection_queue.pop();
            epoll_event ev;
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = client_fd;
            if (epoll_ctl(worker->epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                std::cerr << "Failed to add file descriptor to epoll" << std::endl;
                close(client_fd);
            }
        }
        worker->queue_cv.wait(lock);
    }

}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
    std::cerr << "Failed to create socket" << std::endl;
    return -1;
    }
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    int opt = 1;    // 端口复用
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof opt);

    if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return -1;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return -1;
    }

    if (set_nonblocking(server_fd) < 0) {
        std::cerr << "Failed to set non-blocking" << std::endl;
        return -1;
    }

    std::vector<Worker> workers(NUM_WORKERS);
    ThreadPool thread_pool(NUM_THREADS);

    for (int i = 0; i < NUM_WORKERS; i++) {
        workers[i].epoll_fd = epoll_create1(0);
        if (workers[i].epoll_fd < 0) {
            std::cerr << "Failed to create epoll instance" << std::endl;
            return -1;
        }
        std::thread worker_thread(worker_function, &workers[i], std::ref(thread_pool));
        worker_thread.detach();
    }

    int current_worker = 0;
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            //std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }
        
        if (client_fd > 0) {
            std::cerr << "accept connection" << std::endl;
        }

        if (set_nonblocking(client_fd) < 0) {
            std::cerr << "Failed to set non-blocking" << std::endl;
            close(client_fd);
            continue;
        }

        {
            std::unique_lock<std::mutex> lock(workers[current_worker].queue_mutex);
            workers[current_worker].connection_queue.push(client_fd);
            workers[current_worker].queue_cv.notify_one();
        }

        current_worker = (current_worker + 1) % NUM_WORKERS;
    }

    close(server_fd);
    return 0;
}
