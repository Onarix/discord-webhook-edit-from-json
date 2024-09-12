# Use an official Arch Linux image as the base
FROM archlinux:latest

# Install base dependencies, including git and sudo
RUN pacman -Syu --noconfirm && \
    pacman -S --needed --noconfirm base-devel git sudo curl

# Create a new non-root user
RUN useradd -m builder && echo "builder ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/builder

# Switch to the new non-root user
USER builder
WORKDIR /home/builder

# Clone yay as the non-root user and build it
RUN git clone https://aur.archlinux.org/yay.git && \
    cd yay && \
    makepkg -si --noconfirm

# Switch back to root to clean up and install d++
USER root
RUN rm -rf /home/builder/yay

# Install d++ via yay as the non-root user again
USER builder
RUN yay -S --noconfirm dpp

# Switch back to root for further configuration
USER root

# Set the working directory inside the container
WORKDIR /app

# Copy your application's source code into the container
COPY . /app

# Build your application using g++
RUN g++ -std=c++17 -I/usr/include/dpp -o wh_edit src/wh_edit.cpp -L/usr/lib -ldpp -lcurl -lssl -lcrypto

# Make env.json file template
RUN touch env.json
RUN if [ ! -s env.json ]; then \
    echo '{ \
        "BOT_TOKEN" : "Put your bot token here", \
        "WEBHOOK_URL" : "Put your webhook URL here", \
        "JSON_FILENAME" : "Put the directory of your JSON webhook file" \
    }' > env.json; \
fi

# Set the entrypoint to run your application
CMD ["./wh_edit"]
