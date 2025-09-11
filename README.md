<h1 align=center>ft_irc</h1>
<p align=center>
  <img src="https://github.com/miladrahmat/42-badges/blob/master/badges/ft_irce.png" /><img height="144" alt="Screenshot from 2025-09-11 10-57-09" src="https://github.com/user-attachments/assets/72314041-101d-4f84-b2dc-cf8a317c90eb" />
</p>
<p align=center>
  A simple <b>Internet Relay Chat (IRC) server</b>, that follows the <a href="https://modern.ircdocs.horse/">Modern IRC Client Protocol</a> and supports multiple clients, channels and essential IRC features.
</p>

## Content

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Authors](#authors)


## Introduction

This project is part of the [Hive Helsinki](https://www.hive.fi/) **core curriculum** where we implemented an **Internet Relay Chat (IRC) server** with essential features and multiple client connections.

We did not implement an **IRC client**, but instead used [Irssi](https://irssi.org/) as a reference to connect to the server and test the features while development.

The project is implemented in `C++` with the goal to learn how to implement a server, use sockets and handle client-server communication.

#### What is Internet Relay Chat?

**Internet Relay Chat** or **IRC** is a text-based communication protocol on the Internet. It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.

**IRC clients** connect to **IRC servers** in order to join channels. **IRC servers** are connected toghether to form a network.

## Features

- **Multiple clients** support
- **Nickname change** support
- **Channels** with topic management
- **Private messages** between users
- **Operators** and administrative privileges
- **Authentication** via password
- **Channel modes** (invite-only, topic restrictions etc.)
- **Kicking** users from a channel
- **Invitations** to private channels
- **WHO command** for listing channel users
- **File transfer** between users

### Limitations

- **Capability negotiations** are not supported

## Installation

Clone the repository and compile the server:
```bash
git clone https://github.com/miladrahmat/ft_irc.git
cd ft_irc
make
```

## Usage

Start the server with:
```bash
./ircserv <port> <password>
```
- `<port>`: the **TCP port** the server will listen on (e.g. 6667)
- `<password>`: the password required for clients to authenticate (Cannot be less than 4 characters)

#### Example
```bash
./ircserv 6667 hello
```

### Connecting to the server using Irssi

To install **Irssi**, follow the instructions [here.](https://irssi.org/download/)

After installing **Irssi**, you can connect to the server locally:

```bash
irssi -c 127.0.0.1 -p <port> -w <password>
```
- `127.0.0.1` is the **IP address** of the host machine, this can wary.


Or in the same network:

```bash
irssi -c <host ip> -p <port> -w <password>
```

## Authors
- [Milad Rahmat Abadi](https://www.github.com/miladrahmat)
- [Helmi Pirkola](https://www.github.com/hpirkola)
- [Maria Helenius](https://www.github.com/mariaaallotar)
