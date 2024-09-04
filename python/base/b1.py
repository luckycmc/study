import _thread
import time

def print_name(threaName,delay):
    count = 0
    while count < 5:
        time.sleep(delay)
        count+=1
        print ("%s: %s" % ( threaName,time.ctime(time.time()) ))
  
#创建两个线程
try:
    _thread.start_new_thread(print_name,("Thread-1",2,))     
    _thread.start_new_thread(print_name,("Thread-2",4,))   
except:
    print("Error:无法启动线程!")

while 1:
    pass
       