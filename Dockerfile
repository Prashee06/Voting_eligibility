FROM gcc:13

WORKDIR /app

COPY server.cpp .
COPY httplib.h .
COPY index.html .

RUN g++ server.cpp -o server -std=c++17 -pthread

EXPOSE 9090

CMD ["./server"]