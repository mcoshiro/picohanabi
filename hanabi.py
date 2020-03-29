#!/usr/bin/env python
import socket
import os.path
import subprocess

#GUI via web server
hanabi_port = 125
debug_mode = False
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#server_socket.bind((socket.gethostname(),125)) <- use this (if ports are forwarded) to be viewable externally (not recommended, probably a security concern)
server_socket.bind(('localhost',hanabi_port))
print('Welcome to Picohanabi v 0.2.')
print('Connect by going to localhost:'+str(hanabi_port)+' on a web browser. Picohanabi supports only 1 connection at a time.')
if (debug_mode):
    print('DEBUG: debug mode is ON')
server_socket.listen(1)

#main loop
file_to_read = 'example'
path_to_read = './'
event_to_read = '0'
while True:
    #accept connections and respond- only 1 connection at a time
    (client_socket, address) = server_socket.accept()
    msg = b''
    broken_connection = False
    while True:
        chunk = client_socket.recv(2048)
        msg = msg + chunk
        if chunk == b'':
            if (debug_mode):
                print('DEBUG: ',end='')
                print(msg)
            print("Error:connection broken")
            broken_connection = True
            break
        if (msg[-4:]==b'\r\n\r\n'):
            #end of message?
            if (debug_mode):
                print('DEBUG: ',end='')
                print(msg)
            break
    if (not broken_connection):
        #process browser request
        valid_url = False
        is_favicon = False
        user_request = msg.decode('utf-8')
        request_string = user_request.split()[1]
        if (request_string == '/'):
            path_to_read = './'
            file_to_read = 'example'
            event_to_read = '0'
            valid_url = True
        elif (request_string == '/favicon.ico'):
            is_favicon = True
            valid_url = True
        elif (request_string.split('?')[0] == '/get_event'):
            file_to_read = request_string.split('?')[1].split('&')[0].split('=')[1]
            if (file_to_read[-5:] == '.root'):
                file_to_read = file_to_read[:-5]
            event_to_read = request_string.split('?')[1].split('&')[1].split('=')[1]
            path_to_read = '/'.join(file_to_read.split('%2F')[:-1])
            if (path_to_read == ''):
                path_to_read = './'
            file_to_read = file_to_read.split('%2F')[-1]
            valid_url = True
        elif (request_string == '/previous_event'):
            event_to_read = str(int(event_to_read)-1)
            valid_url = True
        elif (request_string == '/next_event'):
            event_to_read = str(int(event_to_read)+1)
            valid_url = True
        #check if file exists, generate if possible
        if (file_to_read == 'example'):
            event_to_read = '0'
        if (not os.path.isfile('data/'+file_to_read+'_'+event_to_read+'.js')):
            generate_display_return_value = subprocess.call(['bin/generate_display',path_to_read,file_to_read,event_to_read])
            if (generate_display_return_value != 0):
                #failed to generate
                valid_url = False
        #return appropriate webpage
        if (valid_url):
            if (is_favicon):
                client_socket.send(b'')
            #check if file exists
            else:
                html_file = open('site/index_1.html','r')
                html_string = html_file.read()
                #replace button text
                html_string = html_string.replace('%file_to_read%',file_to_read+'.root')
                html_string = html_string.replace('%event_to_read%',event_to_read)
                html_file.close()
                html_file = open('data/'+file_to_read+'_'+event_to_read+'.js','r')
                html_string += html_file.read()
                html_file.close()
                html_file = open('site/index_2.html','r')
                html_string += html_file.read()
                html_file.close()
                reply_string = 'HTTP/1.1 200 OK\r\nDate: Fri, 27 Mar 2020 23:29:05 GMT\r\nServer: hanabi (CERN CentOS 7)\r\nX-Frame-Options: SAMEORIGIN\r\nLast-Modified: Fri, 27 Mar 2020 23:29:05 GMT\r\nETag: "190056-1dd9-594e2e50e6980"\r\nAccept-Ranges: bytes\r\nContent-Length: ' + str(len(html_string)+425) + '\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">\n'
                client_socket.send(reply_string.encode('utf-8'))
                client_socket.send(html_string.encode('utf-8'))
        else:
            #not found
            client_socket.send(b'HTTP/1.1 200 OK\r\nDate: Fri, 27 Mar 2020 23:29:05 GMT\r\nServer: hanabi (CERN CentOS 7)\r\nX-Frame-Options: SAMEORIGIN\r\nLast-Modified: Fri, 27 Mar 2020 23:29:05 GMT\r\nETag: "190056-1dd9-594e2e50e6980"\r\nAccept-Ranges: bytes\r\nContent-Length: 7641\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">\n')
            html_file = open("site/404.html","r")
            html_string = html_file.read()
            client_socket.send(html_string.encode('utf-8'))
            html_file.close()
