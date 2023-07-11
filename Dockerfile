FROM epitechcontent/epitest-docker:latest
WORKDIR /app

RUN dnf install -y clang-devel bear

COPY CMakeLists.txt /app
COPY sources /app/sources

RUN cmake -B build
RUN cmake --build build -j

RUN install ./build/coconut /usr/bin

COPY check.sh /app
ENTRYPOINT ["/app/check.sh"]
