# JUSTCTF2023

Binary Exploitation Challenges repository for JUST CTF 2023.

To facilitate the testing of your challenges in a quick and easy manner, I am providing you the dockerized environment of the challenges.

```bash
$ git clone https://github.com/o54ma-4l5h4r1f/JUSTCTF2023.git

$ cd JUSTCTF2023.git

$ sudo docker-compose up --build -d
```

note the `-d` flag (Detached mode: Run containers in the background) used, so make sure to stops and remove the containers when you done ^^

```bash
$ sudo docker-compose down
```

I have included the solutions of the challenges in the `solve.py` file within each directory, so check them out ^^.

```bash
$ cd <challenge>

$ python3 solve.py

# OR locally

$ python3 solve.py LOCAL 
```



# Notes

## gcc vresion 

If you're using a higher version than 11.3, you should be able to run the challenges without any issues since that was the version of gcc I used to compile them. However, if you're using a lower version, you may encounter errors.

```bash
$ gcc --version 

gcc (Debian 11.3.0-5) 11.3.0
Copyright (C) 2021 Free Software Foundation, Inc.
```

## docker/docker-compose installation

[From Docker Docs](https://docs.docker.com/engine/install/ubuntu/)

```bash
$ sudo apt-get update

$ sudo apt-get install ca-certificates curl gnupg lsb-release

# Add Docker’s official GPG key:
$ sudo mkdir -m 0755 -p /etc/apt/keyrings
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg

# set up the repository:
$ echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# And again
$ sudo apt-get update

$ sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

$ sudo apt-get install docker-compose
```

