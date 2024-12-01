# 베이스 이미지로 최신 Ubuntu 사용
FROM ubuntu:latest

# 필수 패키지 설치
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    libncurses-dev \
    git \
    && apt-get clean

# 작업 디렉토리 설정
WORKDIR /app

# 호스트의 파일을 컨테이너로 복사
COPY . .

# Makefile을 통해 빌드 (필요한 경우 수정)
RUN make

# 기본 실행 명령 (에디터 실행)
CMD ["./viva"]
