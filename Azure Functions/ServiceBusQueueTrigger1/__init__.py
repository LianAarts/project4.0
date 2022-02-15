import logging # for logs and debuging
import azure.functions as func # functions library
from requests import post # library for making API POST
import json # decode and make Json

def main(msg: func.ServiceBusMessage):
    # decode message as json
    message = msg.get_body().decode('utf-8')
    jsonString = json.loads(message)

    # calculate available spots
    # select value with key: availableSpotNumber
    spots = jsonString["availableSpotNumber"] 
    occupied = 12 - spots

    # get timestamp from
    time = str(msg.enqueued_time_utc)
    # get minutes of timestamp
    time = int(time.split(":")[1])

    # only post every 30 minutes
    if  1 <= time <= 32 or 0 <= time <= 2:
        # address for post request    
        url = "https://project40.bravang.com/stats/parkingspot"
        headers = {
            "Authorization": "",
            "content-type": "application/json",
        }
        # json data
        data = {
            "available": spots,
            "occupied": occupied
        }
        # API post
        response = post(url, headers=headers, data=json.dumps(data))
        # log if post is executed and response of post request
        logging.info('executed: %s, response: %s', time, response)
    else:
        # log if post is not executed
        logging.info('not executed: %s', time)

    # log available and occupied spots
    logging.info('occupied spots: %s', occupied)
    logging.info('available spots: %s', spots)