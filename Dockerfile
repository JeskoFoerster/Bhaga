# Use a base image
FROM alpine:latest

# Maintainer information
MAINTAINER Aleksandar Diklic "https://github.com/rastasheep"

# Update package lists and install necessary packages
RUN apt-get update && \
    apt-get install -y openssh-server g++ gdb curl rsync sudo default-jre net-tools git cmake

# Clean up after installation
RUN apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Expose SSH port
EXPOSE 22/tcp

# Create necessary directories
RUN mkdir -p /tmp/Bhaga/cmake-build-dockerbhaga \
    && mkdir /root/.ssh \
    && mkdir /var/run/sshd

# Configure SSH
RUN sed -ri 's/UsePAM yes/#UsePAM yes/g' /etc/ssh/sshd_config && \
    sed -ri 's/^#?PermitRootLogin\s+.*/PermitRootLogin yes/' /etc/ssh/sshd_config

# Set root password
RUN echo 'root:root' | chpasswd

# Add any additional configurations if needed

# Set default command to start SSH
CMD ["/usr/sbin/sshd", "-D"]
