FROM epitechcontent/epitest-docker:latest
WORKDIR /app

RUN dnf install -y clang-devel bear

COPY CMakeLists.txt /app
COPY sources /app/sources

RUN cmake -B build
RUN cmake --build build

COPY coding-style.sh /app
