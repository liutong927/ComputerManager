# generate station name used by 12306
import requests
import re
from pprint import pprint
import io
import sys

# stdout default format is ascii, need to specify utf-8 for Chinese character.
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

url = 'https://kyfw.12306.cn/otn/resources/js/framework/station_name.js?station_version=1.9055'
response = requests.get(url, verify=False)
stations = re.findall(u'([\u4e00-\u9fa5]+)\|([A-Z]+)', response.text)
pprint(dict(stations), indent=4)