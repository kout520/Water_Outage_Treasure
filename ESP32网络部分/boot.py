# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
#import webrepl
#webrepl.start()
from wifi import wifi_connect


# 定义 WIFI 的账号密码
ssid = '222'
password = 'tbjptr74'

# 连接 WiFi
#wifi_connect(ssid, password)