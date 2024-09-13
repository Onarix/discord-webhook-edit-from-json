# discord-webhook-edit-from-json :memo:

## Overview
<b>discord-webhook-edit-from-json</b> is a simple docker C++ application that lets user put changes into their webhooks using JSON webhook configuration file.

## How to run?

### 1) Prerequisites
- docker

### 2) Building app
To build this app run this command:
```console
docker build -t wh_edit .
```

### 3) Files
The program needs 2 .json files to work properly: 
* <b>env.json</b> with this content in it:
```json
{
    "BOT_TOKEN" : "Put your bot token here",
    "WEBHOOK_URL" : "Put your webhook URL here",
    "JSON_FILENAME" : "Put the directory of your JSON webhook file"
}
``` 
<b>NOTE:</b> env.json file must be put in the root directory of the project!

* <b>.json file with webhook info in it, name and directory of it has to match the "JSON_FILENAME" value in env.json


### 4) Running app
Finally you can run the app using:
```console
docker run -it --rm wh_edit
```
