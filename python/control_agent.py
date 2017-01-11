'''
Control Agent: Receive the control command from the board and perform actual action

Dependency:
	PyUserInput
	pybulez
'''
import threading
import time
import bluetooth as bt
from pymouse import PyMouse
from pykeyboard import PyKeyboard

PROMPT_IN = '\033[92m<<\033[0m '
PROMPT_OUT = '\033[94m>>\033[0m '

# M = PyMouse()
K = PyKeyboard()
NAME = "My Linux"

def thread_recv(sock):
    data_pkg = bytearray()
    while 1:
        data = sock.recv(1)
        print("receive" + str(data))
        if len(data) > 0:
            data_pkg.append(data[0])
        if len(data_pkg) >= 4:
            if data_pkg[0] == 0xaa:
                print("key action")
                if data_pkg[1] & 0x01:
                    K.press_key(K.shift_key)
                if data_pkg[1] & 0x02:
                    K.press_key(K.alt_key)
                if data_pkg[1] & 0x04:
                    K.press_key(K.control_key)
                if data_pkg[1] & 0x08:
                    K.press_key(K.windows_l_key)
                if data_pkg[2]:
                    K.tap_key(data_pkg[2])
                if data_pkg[1] & 0x08:
                    K.release_key(K.windows_l_key)
                if data_pkg[1] & 0x04:
                    K.release_key(K.control_key)
                if data_pkg[1] & 0x02:
                    K.release_key(K.alt_key)
                if data_pkg[1] & 0x01:
                    K.release_key(K.shift_key)
            elif data_pkg[0] == 0x55:
                print("Mouse action")
            # print(str(data))
            # for c in str(data):
                # if ord(c) > 0: K.tap_key(str(c))
            data_pkg = bytearray()


def send_name(sock, name):
    buf = bytearray()
    for i, ch in enumerate(name):
        if i % 3 == 0:
            buf.append(0x20 + (i // 3))
        buf.append(ord(ch))
        if i % 3 == 2:
            sock.send(bytes(buf))
            time.sleep(0.1)
            buf.clear()
    if len(buf) > 0:
        while len(buf) < 4:
            buf.append(0)
        sock.send(bytes(buf))
        time.sleep(0.1)


def thread_send(sock):
    buf = bytearray((0xff, 0x02, 0x00, 0xaa))
    i = 0
    while 1:
        sock.send(bytes(buf))
        time.sleep(0.5)
        i = i + 1
        if i % 10 == 0:
            send_name(sock, NAME)


def main():
    sock = bt.BluetoothSocket(proto=bt.RFCOMM)
    print('CONNECTING')
    sock.connect(('20:16:06:29:36:96', 1))
    threading.Thread(target=thread_recv, args=(sock,)).start()
    threading.Thread(target=thread_send, args=(sock,)).start()

if __name__ == '__main__':
    main()
