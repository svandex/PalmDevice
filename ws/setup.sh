#!/bin/bash

firewall-cmd --add-service=http;
firewall-cmd --add-port=9002/tcp;
firewall-cmd --add-port=3306/tcp;
