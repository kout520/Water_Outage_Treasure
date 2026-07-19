import time
import network
from umqttsimple import MQTTClient
from machine import UART, Pin
import ujson
import ntptime


uart2 = UART(2, baudrate=115200, tx=17, rx=16)
temp = 0

# ========== Server酱配置 ==========
SENDKEY = "SCT323451T6pt04uPYbiy2YnJf6ng2boK9"
          

# ========== WiFi配置文件路径 ==========
WIFI_CONFIG_FILE = 'wifi_config.json'



# ========== 修复后的微信通知函数 ==========
def send_wechat(title, content):
    """
    发送微信通知 - 使用正确的表单格式
    """
    try:
        import urequests
        
        url = f"https://sctapi.ftqq.com/{SENDKEY}.send"
        
        # 使用表单格式，不是JSON
        data = f"title={title}&desp={content}"
        
        headers = {
            'Content-Type': 'application/x-www-form-urlencoded'
        }
        
        print(f"正在发送通知: {title}")
        response = urequests.post(url, data=data, headers=headers)
        
        result = response.json()
        print(f"响应: {result}")
        response.close()
        
        if result.get("code") == 0:
            print("✅ 微信通知发送成功！")
            return True
        else:
            print(f"❌ 发送失败: {result.get('message')}")
            return False
            
    except Exception as e:
        print(f"❌ 发送异常: {e}")
        return False

# ========== 测试函数 ==========
def test_wechat():
    send_wechat(
        "测试通知",
        "这是一条测试消息，如果你收到了，说明配置成功！"
    )

# ========== 漏水警报 ==========
def send_leak_alert():
    title = "⚠️ 漏水警报！"
    content = """检测到漏水情况！

时间: 刚刚
位置: 水质监测系统
状态: 紧急

请立即检查并处理！

---
此消息由ESP32自动发送"""
    
    send_wechat(title, content)

# ========== 系统启动通知 ==========
def send_startup_notification():
    title = "🚀 系统启动"
    content = """水质监测系统已启动

启动时间: 刚刚
MQTT服务器: 159.75.161.237
通知服务: 已激活

系统正在运行中..."""

    send_wechat(title, content)


SENDKEY_FILE = 'sendkey.txt'

def load_sendkey():
    try:
        with open(SENDKEY_FILE, 'r') as f:
            key = f.read().strip()
            if key:
                print(f"✅ 加载已保存的SENDKEY")
                return key
    except:
        pass
    return "SCT323451T6pt04uPYbiy2YnJf6ng2boK9"  # 默认值

def save_sendkey(key):
    try:
        with open(SENDKEY_FILE, 'w') as f:
            f.write(key)
        print(f"✅ SENDKEY 已保存")
    except Exception as e:
        print(f"❌ 保存SENDKEY失败: {e}")




# ========== NTP时间同步 ==========
def sync_time():
    """通过NTP同步时间，带重试"""
    for i in range(3):
        try:
            ntptime.host = "ntp.aliyun.com"  # 国内服务器更稳定
            ntptime.settime()                 # 同步UTC时间
            
            # UTC+8 转换为北京时间
            t = time.time() + 8 * 3600
            tm = time.localtime(t)
            # 手动设置RTC
            from machine import RTC
            rtc = RTC()
            rtc.datetime((tm[0], tm[1], tm[2], tm[6], tm[3], tm[4], tm[5], 0))
            return True
        except Exception as e:
            print(f"⚠️ NTP同步失败({i+1}/3): {e}")
            time.sleep(1)
    return False

# ========== 加载WiFi配置 ==========
def load_wifi_config():
    """从文件加载WiFi配置"""
    try:
        with open(WIFI_CONFIG_FILE, 'r') as f:
            config = ujson.load(f)
            print(f"✅ 加载已保存的WiFi配置: {config['ssid']}")
            return config
    except:
        print("⚠️ 未找到已保存的WiFi配置，使用默认配置")
        return {
            'ssid': '252',
            'password': 'jfgdjjgj'
        }

# ========== 保存WiFi配置 ==========
def save_wifi_config(ssid, password):
    """保存WiFi配置到文件"""
    try:
        config = {
            'ssid': ssid,
            'password': password
        }
        with open(WIFI_CONFIG_FILE, 'w') as f:
            ujson.dump(config, f)
        print(f"✅ WiFi配置已保存: {ssid}")
        return True
    except Exception as e:
        print(f"❌ 保存WiFi配置失败: {e}")
        return False

# ========== WiFi配置 ==========
wifi_config = load_wifi_config()
last_connected_wifi = None  # 记录最后成功连接的WiFi

# ========== WiFi配置处理 ==========
def handle_wifi_config(cmd):
    """处理WiFi配置命令 - 一次性接收格式"""
    global wifi_config
    
    # 处理格式: @WIFI:ssid,password
    if cmd.startswith('@WIFI:'):
        try:
            wifi_data = cmd[6:].strip()  # 去掉 @WIFI:
            if ',' in wifi_data:
                ssid, password = wifi_data.split(',', 1)
                new_ssid = ssid.strip()
                new_password = password.strip()
                
                print(f"\n{'='*50}")
                print(f"✅ 收到WiFi配置:")
                print(f"  SSID: {new_ssid}")
                print(f"  密码: {'*' * len(new_password)}")
                print(f"{'='*50}\n")
                
                # 更新配置
                wifi_config['ssid'] = new_ssid
                wifi_config['password'] = new_password
                
                uart2.write(b'WiFi配置已接收，开始连接...\n')
                return 'connect'
            else:
                print("❌ WiFi格式错误")
                return False
        except Exception as e:
            print(f"❌ WiFi解析错误: {e}")
            return False
    
    return False

# ========== UART处理（WiFi配置阶段）==========
def uart2_take_config():
    """WiFi配置阶段的UART处理"""
    if uart2.any():
        try:
            data = uart2.readline()
            if data:
                cmd = data.decode('utf-8').strip()
                if cmd:
                    print(f"📥 收到: {cmd}")
                    return handle_wifi_config(cmd)
        except Exception as e:
            print(f"❌ UART错误: {e}")
    return False

# ========== UART处理（运行时）==========
def uart2_take_runtime():
    """运行时的UART处理"""
    global temp
    
    if uart2.any():
        try:
            data = uart2.readline()
            if data:
                cmd = data.decode('utf-8').strip()
                print(f"收到命令: {cmd}")
                
                # 检查是否是WiFi配置命令
                result = handle_wifi_config(cmd)
                if result == 'connect':
                    return 'reconnect_wifi'
               

                # 收到新key时
                if cmd.startswith('@SERVE:'):
                    new_key = cmd[7:].strip()
                    if new_key:
                        SENDKEY = new_key
                        save_sendkey(SENDKEY)  # 持久化

                # 原有的打卡命令
                if cmd.upper() == '@1':

                    temp = 13
                elif cmd.upper() == '@2':

                    temp = 23
                elif cmd.upper() == '@3':

                    temp = 33
                elif cmd.upper() == '@4':

                    temp = 12
                elif cmd.upper() == '@5':

                    temp = 23
                elif cmd.upper() == '@6':

                    temp = 31
        except Exception as e:
            print(f"❌ UART错误: {e}")
    
    return None

# ========== WiFi连接函数 ==========
def do_connect(ssid=None, password=None, max_wait=30):
    """WiFi连接函数"""
    global wifi_config, last_connected_wifi
    
    if ssid is None:
        ssid = wifi_config['ssid']
    if password is None:
        password = wifi_config['password']
    
    wlan = network.WLAN(network.STA_IF)
    
    # 确保WiFi接口激活
    if not wlan.active():
        wlan.active(True)
        time.sleep(1)
    
    retry_count = 0
    max_retries = 3
    
    while retry_count < max_retries:
        try:
            # 如果已连接到目标WiFi，直接返回
            if wlan.isconnected():
                current_ssid = wlan.config('essid')
                if current_ssid == ssid:
                    print(f'✅ 已连接到目标WiFi: {ssid}')
                    return wlan
            
            retry_count += 1
            print(f"\n{'='*50}")
            print(f"第 {retry_count}/{max_retries} 次尝试连接WiFi")
            print(f"SSID: {ssid}")
            print(f"{'='*50}")
            
            # 断开现有连接
            try:
                if wlan.isconnected():
                    wlan.disconnect()
                    time.sleep(1)
            except:
                pass
            
            # 重新激活接口
            wlan.active(False)
            time.sleep(0.5)
            wlan.active(True)
            time.sleep(1)
            
            # 开始连接
            wlan.connect(ssid, password)
            
            wait_time = 0
            while not wlan.isconnected() and wait_time < max_wait:
                # 在连接过程中检查配置命令
                result = uart2_take_config()
                if result == 'connect':
                    print("🔄 使用新配置重新连接...")
                    return do_connect(wifi_config['ssid'], wifi_config['password'], max_wait)
                
                print(f"连接中... {wait_time}/{max_wait}s")
                time.sleep(1)
                wait_time += 1
            
            if wlan.isconnected():
                print('✅ WiFi连接成功!')
                print('IP地址:', wlan.ifconfig()[0])
                uart2.write(b'@2\r\n')
                
                # 保存成功的WiFi配置
                save_wifi_config(ssid, password)
                
                # 记录最后成功连接的WiFi
                last_connected_wifi = {
                    'ssid': ssid,
                    'password': password
                }
                sync_time()  # ← 加在这里
                return wlan
            else:
                print(f"❌ 连接超时")
                
                if retry_count < max_retries:
                    print("5秒后重试...")
                    for i in range(5):
                        result = uart2_take_config()
                        if result == 'connect':
                            return do_connect(wifi_config['ssid'], wifi_config['password'], max_wait)
                        time.sleep(1)
                
        except Exception as e:
            print(f"❌ WiFi错误: {e}")
            # 重置WiFi接口
            try:
                wlan.active(False)
                time.sleep(1)
                wlan.active(True)
                time.sleep(1)
            except:
                pass
    
    print(f"\n❌ WiFi连接失败")
    return None

# ========== 尝试切换WiFi（带回退功能）==========
def try_switch_wifi(new_ssid, new_password, timeout=6):
    """尝试切换到新WiFi，失败后回退到之前的WiFi"""
    global last_connected_wifi, wifi_config
    
    # 保存当前WiFi信息（用于回退）
    backup_wifi = last_connected_wifi.copy() if last_connected_wifi else None
    
    print(f"\n{'='*50}")
    print(f"🔄 尝试切换WiFi")
    print(f"  当前: {backup_wifi['ssid'] if backup_wifi else '无'}")
    print(f"  目标: {new_ssid}")
    print(f"{'='*50}\n")
    
    wlan = network.WLAN(network.STA_IF)
    
    # 确保WiFi接口激活
    if not wlan.active():
        wlan.active(True)
        time.sleep(1)
    
    try:
        # 断开当前连接
        if wlan.isconnected():
            print("断开当前WiFi连接...")
            wlan.disconnect()
            time.sleep(1)
        
        # 重新激活接口
        wlan.active(False)
        time.sleep(0.5)
        wlan.active(True)
        time.sleep(1)
        
        # 尝试连接新WiFi
        print(f"正在连接新WiFi: {new_ssid}")
        wlan.connect(new_ssid, new_password)
        
        # 等待连接（最多timeout秒）
        wait_time = 0
        while not wlan.isconnected() and wait_time < timeout:
            print(f"连接中... {wait_time}/{timeout}s")
            time.sleep(1)
            wait_time += 1
        
        if wlan.isconnected():
            print(f"✅ 成功切换到新WiFi: {new_ssid}")
            print(f"IP地址: {wlan.ifconfig()[0]}")
            uart2.write(b'WiFi切换成功!\n')
            
            # 保存新WiFi配置
            save_wifi_config(new_ssid, new_password)
            wifi_config['ssid'] = new_ssid
            wifi_config['password'] = new_password
            
            # 更新最后成功连接的WiFi
            last_connected_wifi = {
                'ssid': new_ssid,
                'password': new_password
            }
            return wlan, True
        else:
            print(f"❌ 连接新WiFi超时")
            uart2.write(b'WiFi切换失败，回退到原WiFi...\n')
            
            # 回退到之前的WiFi
            if backup_wifi:
                print(f"\n🔙 回退到之前的WiFi: {backup_wifi['ssid']}")
                
                # 断开失败的连接
                try:
                    wlan.disconnect()
                    time.sleep(1)
                except:
                    pass
                
                # 重新激活接口
                wlan.active(False)
                time.sleep(0.5)
                wlan.active(True)
                time.sleep(1)
                
                # 连接回之前的WiFi
                wlan.connect(backup_wifi['ssid'], backup_wifi['password'])
                
                wait_time = 0
                while not wlan.isconnected() and wait_time < 10:
                    print(f"回退连接中... {wait_time}/10s")
                    time.sleep(1)
                    wait_time += 1
                
                if wlan.isconnected():
                    print(f"✅ 成功回退到: {backup_wifi['ssid']}")
                    uart2.write(b'已回退到原WiFi\n')
                    
                    # 恢复配置
                    wifi_config['ssid'] = backup_wifi['ssid']
                    wifi_config['password'] = backup_wifi['password']
                    
                    return wlan, False
                else:
                    print(f"❌ 回退失败")
                    uart2.write(b'回退失败\n')
                    return None, False
            else:
                print("⚠️ 没有可回退的WiFi")
                return None, False
                
    except Exception as e:
        print(f"❌ WiFi切换错误: {e}")
        uart2.write(b'WiFi切换出错\n')
        
        # 尝试回退
        if backup_wifi:
            print(f"🔙 尝试回退...")
            try:
                wlan.active(False)
                time.sleep(0.5)
                wlan.active(True)
                time.sleep(1)
                wlan.connect(backup_wifi['ssid'], backup_wifi['password'])
                time.sleep(5)
                
                if wlan.isconnected():
                    print(f"✅ 回退成功")
                    wifi_config['ssid'] = backup_wifi['ssid']
                    wifi_config['password'] = backup_wifi['password']
                    return wlan, False
            except:
                pass
        
        return None, False
    
    
# ========== 获取当前时间字符串 ==========
def get_time_str():
    """获取当前时间，格式: HH:MM:SS"""
    t = time.localtime()
    return "{:04d}-{:02d}-{:02d}&{:02d}:{:02d}:{:02d}".format(t[0], t[1], t[2],t[3], t[4], t[5])


# ========== 定时发送时间到UART2 ==========
def send_time_to_uart():
    """发送当前时间和日期到UART2"""
    time_str = "@" + get_time_str() + "\n"
    uart2.write(time_str.encode('utf-8'))




# ========== 发送MQTT数据 ==========
def send_data():
    global c
    try:
        if c:
            c.publish("aa", "%s" % temp)
            print(f"发送MQTT: {temp}")
    except Exception as e:
        print(f"❌ MQTT发送失败: {e}")

# ========== MQTT回调 ==========
def sub_cb(topic, msg):
    try:
        topic_str = topic.decode('utf-8')
        msg_str = msg.decode('utf-8')
        print(f"收到MQTT - 主题: {topic_str}, 消息: {msg_str}")
        
        if topic_str == "ledctl":
            if msg_str == "on":
                uart2.write(b'@g\n')
                return
            elif msg_str == "off":
                uart2.write(b'@h\n')
                return
            
            parts = msg_str.split(':')
            if len(parts) >= 3:
                hour = parts[1]
                minute = parts[2]
                second = parts[3]
                
                # ✅ 发送格式: @16:20:21\n
                time_str = f"@{hour}:{minute}:{second}\n"
                uart2.write(time_str.encode('utf-8'))
                print(f"串口发送: {repr(time_str)}")
                
    except Exception as e:
        print(f"回调错误: {e}")

# ========== 主程序 ==========
print("\n" + "="*50)
print("打卡记录系统启动")
print("="*50 + "\n")

c = None
wlan = None

# 连接WiFi
print("🔌 正在连接WiFi...")
wlan = do_connect()

while not wlan or not wlan.isconnected():
    print("\n⚠️ WiFi未连接，等待配置...")
    print("请从串口屏发送WiFi信息")
    
    while not wlan or not wlan.isconnected():
        result = uart2_take_config()
        if result == 'connect':
            wlan = do_connect()
            if wlan and wlan.isconnected():
                break
        time.sleep(0.1)

print("\n✅ WiFi连接成功\n")

# 连接MQTT
print("🔌 正在连接MQTT...")
mqtt_connected = False
mqtt_retry = 0

while not mqtt_connected and mqtt_retry < 5:
    try:
        c = MQTTClient("umqtt_client", "159.75.161.237")
        c.set_callback(sub_cb)
        c.connect()
        c.subscribe(b"ledctl")
        mqtt_connected = True
        print("✅ MQTT已连接\n")
    except Exception as e:
        mqtt_retry += 1
        print(f"❌ MQTT连接失败 ({mqtt_retry}/5): {e}")
        time.sleep(3)
# 启动时
SENDKEY = load_sendkey()
print(SENDKEY)
print("✅ 系统启动完成")
print("="*50 + "\n")
#send_startup_notification()
# 主循环
i = 0
x = 0
wifi_disconnected = False
reconnect_delay = 0

while True:
    try:
        # 检查WiFi状态（添加空值检查）
        if wlan is None or not wlan.isconnected():
            if not wifi_disconnected:
                print("\n⚠️ WiFi断开！")
                wifi_disconnected = True
                reconnect_delay = 0
            
            # 添加重连延迟，避免频繁重连
            if reconnect_delay <= 0:
                print("🔄 尝试重新连接WiFi...")
                uart2.write(b'@1\r\n')
                wlan = do_connect()
                reconnect_delay = 10  # 失败后等待10个循环周期再重试
            else:
                reconnect_delay -= 1
            # 检查重连是否成功
            if wlan and wlan.isconnected():
                wifi_disconnected = False
                reconnect_delay = 0
                print("✅ WiFi重连成功，重连MQTT...")
                try:
                    c = MQTTClient("umqtt_client", "159.75.161.237")
                    c.set_callback(sub_cb)
                    c.connect()
                    c.subscribe(b"ledctl")
                    sync_time()  # ← 加在这里
                    print("✅ MQTT重连成功\n")
                except Exception as e:
                    print(f"❌ MQTT重连失败: {e}")
        
        # 正常运行（只在WiFi连接时执行）
        if wlan and wlan.isconnected():
            if c:
                try:
                    c.check_msg()
                except:
                    pass
            
            # 处理UART命令（包括WiFi配置）
            result = uart2_take_runtime()
            if result == 'reconnect_wifi':
                print("🔄 收到新WiFi配置，尝试切换...")
                
                # 使用带回退功能的WiFi切换
                new_wlan, success = try_switch_wifi(
                    wifi_config['ssid'], 
                    wifi_config['password'], 
                    timeout=6
                )
                
                if new_wlan:
                    wlan = new_wlan
                    
                    if success:
                        # 切换成功，重连MQTT
                        print("重新连接MQTT...")
                        try:
                            c = MQTTClient("umqtt_client", "159.75.161.237")
                            c.set_callback(sub_cb)
                            c.connect()
                            c.subscribe(b"ledctl")
                            print("✅ MQTT重连成功\n")
                        except Exception as e:
                            print(f"❌ MQTT重连失败: {e}")
                    else:
                        # 回退成功，MQTT可能还能用
                        print("检查MQTT连接...")
                        try:
                            if c:
                                c.ping()
                                print("✅ MQTT连接正常\n")
                        except:
                            print("重新连接MQTT...")
                            try:
                                c = MQTTClient("umqtt_client", "159.75.161.237")
                                c.set_callback(sub_cb)
                                c.connect()
                                c.subscribe(b"ledctl")
                                print("✅ MQTT重连成功\n")
                            except Exception as e:
                                print(f"❌ MQTT重连失败: {e}")
                else:
                    print("⚠️ WiFi切换和回退都失败，等待重连...")
            
            if temp != 0:
                send_data()
                temp = 0
            
            i += 1
            if x == 0 or x%10 ==0:
                send_time_to_uart()
                x=1
            if i >= 200:
                i = 0
                x += 1
                if c:
                    try:
                        c.ping()
                    except:
                        pass
        time.sleep(0.05)
    except KeyboardInterrupt:
        print("\n程序中断")
        break
    except Exception as e:
        print(f"❌ 运行错误: {e}")
        time.sleep(1)



