# esp32-ota
Example code for a neat OTA update ESP32 web server:
### First use
- Replace your credentials at [line 11-12 in `ota_example.ino`](/ota_example/ota_example.ino#L11-L12)
- Upload the code to your ESP32 with any C-based compiler.
- Then wait for it to boot.  
- And go to [esp32.local](http://esp32.local/). On Windows you might need to install Apple's [Bonjour Print Service](https://support.apple.com/kb/DL999) to use mDNS.
- This is not a secure login, but partially obfuscated: username and password are `program`
- Click on "Login". You'll be sent to a URL. Make sure not to share it or let people access your browser history (if they have the URL, they can access the login, and upload programs).
### Make new sketch
- Outside of code
  - Make a new sketch like normal.
  - Copy [ota.h](/ota_example/ota.h) and [otaroute.h](/ota_example/otaroute.h) into your sketch's directory.
- Code, before `setup`
  - First, add these to the top of your code:
```c++
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <WebServer.h>
#include "ota.h"
```
  - Then, after that, make sure you declare these:
```c++
WebServer server(80);
const char* host = "esp32";
const char* ssid = "yourNetworkName";
const char* password = "yourNetworkPassword";
```
  - Modify `host` and `ssid`
- Inside of `setup`
  - Set up `WiFi` and make sure you connect to it inside of your `setup`.
  - Also inside your `setup`, after that, do a `#include "otaroute.h"`.
- Inside of `loop`
  - When you have spare time, call `server.handleClient();`. I have an example here waiting for 20ms each loop:
```c++
while (millis() - tt < 10) {
  server.handleClient();
}
```
  - Or you can do it on a seperate core:
```c++
// Check for updates loop, outside of setup:
TaskHandle_t Task;
void handleServer(void * pvParameters) {
  while(true) {
    server.handleClient();
  }
}
// Inside of setup:
xTaskCreatePinnedToCore(
  handleServer,
  "handleServer",
  10000,
  NULL,
  1,
  &Task,
  (xPortGetCoreID() == 1) ? 0 : 1);
```


### Upload sketch
Remember to follow the above guidelines for creating a sketch. **If you forget** and upload a non-OTA capable sketch, you'll **lose OTA access** until another one with OTA is uploaded.
- First, make sure you're logged in:
  - Go to [esp32.local](http://esp32.local/). On Windows you might need to install Apple's [Bonjour Print Service](https://support.apple.com/kb/DL999) to use mDNS.
  - This is not a secure login, but partially obfuscated: username and password are `program`. (Feel free to fork and send a PR with more secure functionality!)
  - Click on "Login". You'll be sent to a URL. Make sure not to share it or let people access your browser history.
- Then build your sketch:
  - Arduino IDE:  
![Choose Sketch, then Export compiled binary](/exportbin.png)
  - Platform IO:
    - Choose compile
    - Once compiled, find `.pio/build/your_build_name_here/`
- Choose `Choose File`
- Select your `.bin` file
- Choose `Upload`
- Don't close the tab, and eventually it'll finish!

**Any errors?** [Let me know.](https://github.com/KTibow/esp32-ota/issues/new)
