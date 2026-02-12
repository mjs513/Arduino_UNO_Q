# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from weather_brick import WeatherForecast
from arduino.app_utils import *

forecaster = WeatherForecast()

last_data_list: list[float] = {}

def get_weather_forecast(city: str) -> str:
    global last_data_list
    forecast = forecaster.get_forecast_by_city(city)
    print(f"Weather forecast for {city}: {forecast.description}: {forecast.cur_temp}: {forecast.daily_precip}: {forecast.cur_wind_speed}: {forecast.cur_wind_dir}")
    #print(f"Weather forecast for {city}: {forecast.description}")
    last_data_list = [forecast.cur_temp, forecast.daily_precip, forecast.cur_wind_speed, float(forecast.cur_wind_dir)]
    #print(last_data_list)
    return forecast.category

def get_weather_data() -> list:
    global last_data_list
    return last_data_list

Bridge.provide("get_weather_forecast", get_weather_forecast)

Bridge.provide("get_weather_data", get_weather_data)

App.run()
