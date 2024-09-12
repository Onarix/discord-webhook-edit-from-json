# discord-webhook-edit-from-json :memo:

## Overview
<b>discord-webhook-edit-from-json</b> is a simple docker C++ application that lets user put changes into their webhooks using JSON webhook configuration file.

## How to run?

### Prerequisites
- docker

### Building app
To build this app run this command:
```console
docker build -t wh_edit .
```

### Running app
To run this app, you have to put your webhook json file in the root directory and fill env.json file with your credentials.
Then you can run it using:
```console
docker run -it --rm wh_edit
```
