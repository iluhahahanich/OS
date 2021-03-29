#pragma once

#include <queue>
#include <mutex>

template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) {
        _size = size;
    }

    void Send(T value) {
        std::unique_lock<std::mutex> ul(_mutex);

        if (_closed) {
            throw std::runtime_error("channel is closed");
        }

        if (_q.size() == _size) {
            _cvSend.wait(ul, [&]() { return _q.size() < _size || _closed ; });
        }

        if (_q.size() < _size) {
            _q.push(value);
        }

        _cvRecv.notify_all();
    }

    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> ul(_mutex);

        if (_q.size() == 0) {
            if (_closed) {
                return {T(), false};
            } else {
                _cvRecv.wait(ul, [&]() { return _q.size() > 0 || _closed; });
                if (_q.size() == 0) {
                    return {T(), false};
                }
            }
        }

        T value = _q.front();
        _q.pop();

        _cvSend.notify_all();

        return {value, true};
    }

    void Close() {
        std::lock_guard<std::mutex> lg(_mutex);
        _closed = true;
        _cvSend.notify_all();
        _cvRecv.notify_all();
    }

private:
    std::queue<T> _q;
    std::mutex _mutex;
    std::condition_variable _cvSend, _cvRecv;
    bool _closed = false;
    int _size = 0;
};