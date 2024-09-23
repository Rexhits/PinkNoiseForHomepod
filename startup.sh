#!/bin/bash

cd homepod-web-app
nohup bash -c 'npm start' &
cd ..

cd homepod-backend
nohup bash -c 'node server.js' &
cd ..

./build/Pinknoise4Homepod&