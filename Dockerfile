# ���̽� �̹����� �ֽ� Ubuntu ���
FROM ubuntu:latest

# �ʼ� ��Ű�� ��ġ
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    libncurses-dev \
    git \
    && apt-get clean

# �۾� ���丮 ����
WORKDIR /app

# ȣ��Ʈ�� ������ �����̳ʷ� ����
COPY . .

# Makefile�� ���� ���� (�ʿ��� ��� ����)
RUN make

# �⺻ ���� ��� (������ ����)
CMD ["./viva"]
