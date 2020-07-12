while(!MDNS.begin(host));
MDNS.addService("http", "tcp", 80);
server.on("/", HTTP_GET, []() {
server.sendHeader("Connection", "close");
server.send(200, "text/html", loginIndex);
});
server.on("/serverIndex", HTTP_GET, []() {
server.sendHeader("Connection", "close");
server.send(200, "text/html", serverIndex);
});
server.on("/update", HTTP_POST, []() {
server.sendHeader("Connection", "close");
server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
ESP.restart();
}, []() {
HTTPUpload& upload = server.upload();
if (upload.status == UPLOAD_FILE_START) {
Serial.printf("Update: %s\n", upload.filename.c_str());
if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
Update.printError(Serial);
}
} else if (upload.status == UPLOAD_FILE_WRITE) {
if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
Update.printError(Serial);
}
} else if (upload.status == UPLOAD_FILE_END) {
if (Update.end(true)) {
Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
} else {
Update.printError(Serial);
}
}
});
server.begin();
