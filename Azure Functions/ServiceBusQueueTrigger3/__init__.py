import logging # for logs and debuging
import azure.functions as func # functions library
from requests import post # library for making API POST
import json # decode and make Json

def main(msg: func.ServiceBusMessage):
    # decode message as json
    serviceBusMessage = msg.get_body().decode('utf-8')
    Fulljson = json.loads(serviceBusMessage)

    # get json data
    rawData = Fulljson["data"]

    # decode json
    data = json.loads(rawData)
    # get length of json
    length = len(data)

    sensor = data[0]["bn"]
    logging.info('sensor: %s', sensor)
    i = 0
    
    #run trough json and print values
    while i < length:
        n = data[i]["n"]
        if n == "temperature":
            temperature = data[i]["v"]
        elif n == "humidity":
            humidity = data[i]["v"]
        elif n == "pressure":
            presure = data[i]["v"]
        elif n == "CO2":
            CO2 = data[i]["v"]
        elif n == "TVOC":
            TVOC = data[i]["v"]
        elif n == "uvIndex":
            uvIndex = data[i]["v"]
        i += 1

    logging.info('temperature: %s\nhumidity: %s\npressure: %s\nCO2: %s\nTVOC: %s\nuvIndex: %s\n',temperature,humidity,presure,CO2,TVOC,uvIndex)

    # url = "http://"
    # headers = {
    #     "Authorization": "",
    #     "content-type": "application/json",
    # }
    # data = {
    #     "entity_id": ""
    # }
    # response = post(url, headers=headers, data=json.dumps(data))