const { createServer } = require('node:http');
const dgram = require('dgram');

const port = 7789;
const hostname = 'localhost';

const udpClient = dgram.createSocket('udp4');

const server = createServer((req, res) => {
    console.log(req.url);
    udpClient.send(req.url, 0, req.url.length, 7878, '127.0.0.1')
    res.statusCode = 200;
    res.setHeader('Content-Type', 'text/plain');
    res.end('ack');
});

server.listen(port, hostname, () => {
    console.log(`Homepod server is running on port ${port}`);
});