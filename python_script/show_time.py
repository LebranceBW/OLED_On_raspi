#!/usr/bin/python3
#encoding:utf-8
import time
import os
import threading
from urllib import request
from bs4 import BeautifulSoup

os.system("OLED -i")
os.system("OLED -l")
Locked = False

def task_time():
        global Locked
        while Locked:
                pass
        Locked=True
        os.system("OLED -t "+time.strftime("%H:%M:%S"))
        Locked=False

def show_weather():
        global Locked
        url = "http://www.msn.com/en-us/weather/today/Chengdu,Sichuan,China/we-city?iso=CN&savedegree=true&weadegreetype=C&el=V9KugR%2F7vc%2F5dT3mQOFubQ%3D%3D"
        response = request.urlopen(url)
        html = response.read()
        html = html.decode("utf-8")
        soup = BeautifulSoup(html, "html5lib")
    
        temperature = soup.find('span', class_='current').string
        weather = soup.find('div', class_='weather-info').span.string.replace(" ", "_")
        while Locked:
                pass
        Locked = True
        os.system("OLED -w "+weather+" "+temperature)
        Locked = False

def ontime(task, interval):
        def func():
                while True:
                        task()
                        time.sleep(interval)
        return func    

t1 = threading.Thread(target=ontime(task_time, 1), name="更新时间r")
t2 = threading.Thread(target=ontime(show_weather, 1600), name="更新天气")
t1.start()
t2.start()
t1.join()
t2.join()

