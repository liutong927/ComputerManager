# 12306 ticket query via command line.

"""
Usage:
    ticket_query_cli.py [-gdtkz] <from> <to> <date>

Options:
    -h,--help   帮助
    -g          高铁
    -d          动车
    -t          特快
    -k          快速
    -z          直达
"""

from docopt import docopt
from station_dict import stations
import requests
from requests.packages.urllib3.exceptions import InsecureRequestWarning
from prettytable import PrettyTable

def cli():
    # parse commnad line to args.
    arguments = docopt(__doc__)
    #print(arguments)
    from_station = stations.get(arguments['<from>'])
    to_station = stations.get(arguments['<to>'])
    date = arguments['<date>']
    #print(from_station, to_station, date)

    # compose query url
    requests.packages.urllib3.disable_warnings(InsecureRequestWarning)
    url = 'https://kyfw.12306.cn/otn/leftTicket/query?leftTicketDTO.train_date={}' \
          '&leftTicketDTO.from_station={}' \
          '&leftTicketDTO.to_station={}' \
          '&purpose_codes=ADULT'.format(date, from_station, to_station)

    #print url to see if it is valid url.
    print(url)

    result = requests.get(url, verify = False)
    raw_trains = result.json()['data']['result']

    #print one train record to understanding its format.
    #print(raw_trains[1])

    # block0|block1-预订|block2-55000G707290|block3-G7072（车次）|block4-SHH(始发）|block5-AQH（终点）|
    # block6-SHH（出发）|block7-NKH（到达）|block8-05:48（出发时间）|block9-07:57（到达时间）|
    # block10-02:09（历时）|block11-Y|block12|block13-20180611|block14-3|
    # block15-HZ|block16-01|block17-08|block18-1|block19-0|block20|block21|
    # block22|block23|block24|block25|block26-无（无座）|block27|
    # block28|block29|block30-有（二等）|block31-有（一等）|block32-16（商务）|
    # block33|block34-O090O0M0|block35-O9OM|block36-1

    # so what we need is data from block 3/6/7/8/9/10/26/30/31/32
    # also note this encode may change with 12306.

    query_options = ''.join([key for key, value in arguments.items() if value is True])

    train_info_parser(raw_trains, query_options).pretty_print()


class train_info_parser:
    header = '车次 车站 时间 历时 商务 一等 二等 无座'.split()

    def __init__(self, raw_trains, query_options):
        self.raw_trains = raw_trains
        self.query_options = query_options

    # train record for display.
    @property
    def trains_info(self):
        for train in self.raw_trains:
            train_info = train.split('|')

            train_code = train_info[3]
            train_type = train_code[0].lower()

            # get train info match the input options.
            if train_type in self.query_options:
                # transfter '' to string '--', the raw info may be ...||...
                # todo: not work?
                for train_info_elem in train_info:
                    if train_info_elem is '':
                        train_info_elem = '--'

                train_start_station = train_info[6]
                train_arrive_station = train_info[7]
                train_start_time = train_info[8]
                train_arrive_time = train_info[9]
                train_duration = train_info[10]
                train_business_seat = train_info[32]
                train_1st_seat = train_info[31]
                train_2st_seat = train_info[30]
                train_no_seat = train_info[26]

                # information transform for display.
                train_station = '\n'.join([train_start_station, train_arrive_station])
                train_time = '\n'.join([train_start_time, train_arrive_time])

                train_info_display = [train_code,train_station,train_time,train_duration,\
                      train_business_seat,train_1st_seat,train_2st_seat,train_no_seat]

                yield train_info_display


    def pretty_print(self):
        pt = PrettyTable()
        pt._set_field_names(self.header)
        for train in self.trains_info:
            pt.add_row(train)
        print(pt)

if __name__ == '__main__':
    cli()