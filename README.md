# MqttInflux
Mqtt daemon to log mqtt message into InfluxDb.

## Introduction
This daemon log mqtt message or value into timeseries database InfluxDb.

## Features
 - Settings influx server
 - Settings each topic
 - TO DO support json message

## Building
git submodule update --init --recursive
cmake .
make

## Licence
MqttInflux is free software : you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

MqttInflux is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with MqttInflux. If not, see http://www.gnu.org/licenses/.
