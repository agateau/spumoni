#!/usr/bin/env python3
"""
Spumoni app to show the weather, using https://wttr.in API.
"""
import argparse
import json
import locale
import urllib.request
import sys
import webbrowser


WWO_CODE = {
    "113": "Sunny",
    "116": "PartlyCloudy",
    "119": "Cloudy",
    "122": "VeryCloudy",
    "143": "Fog",
    "176": "LightShowers",
    "179": "LightSleetShowers",
    "182": "LightSleet",
    "185": "LightSleet",
    "200": "ThunderyShowers",
    "227": "LightSnow",
    "230": "HeavySnow",
    "248": "Fog",
    "260": "Fog",
    "263": "LightShowers",
    "266": "LightRain",
    "281": "LightSleet",
    "284": "LightSleet",
    "293": "LightRain",
    "296": "LightRain",
    "299": "HeavyShowers",
    "302": "HeavyRain",
    "305": "HeavyShowers",
    "308": "HeavyRain",
    "311": "LightSleet",
    "314": "LightSleet",
    "317": "LightSleet",
    "320": "LightSnow",
    "323": "LightSnowShowers",
    "326": "LightSnowShowers",
    "329": "HeavySnow",
    "332": "HeavySnow",
    "335": "HeavySnowShowers",
    "338": "HeavySnow",
    "350": "LightSleet",
    "353": "LightShowers",
    "356": "HeavyShowers",
    "359": "HeavyRain",
    "362": "LightSleetShowers",
    "365": "LightSleetShowers",
    "368": "LightSnowShowers",
    "371": "HeavySnowShowers",
    "374": "LightSleetShowers",
    "377": "LightSleet",
    "386": "ThunderyShowers",
    "389": "ThunderyHeavyRain",
    "392": "ThunderySnowShowers",
    "395": "HeavySnowShowers",
}

ICON_FOR_WWO = {
    "Cloudy": "clouds",
    "Fog": "fog",
    "HeavyRain": "showers-day",
    "HeavyShowers": "showers",
    "HeavySnow": "snow",
    "HeavySnowShowers": "snow",
    "LightRain": "showers-scattered",
    "LightShowers": "showers-scattered-day",
    "LightSleet": "snow-rain",
    "LightSleetShowers": "snow-rain",
    "LightSnow": "snow-scattered",
    "LightSnowShowers": "snow-scattered-day",
    "PartlyCloudy": "few-clouds",
    "Sunny": "clear",
    "ThunderyHeavyRain": "storm",
    "ThunderyShowers": "storm-day",
    "ThunderySnowShowers": "snow",
    "VeryCloudy": "many-clouds",
}


def icon_for_code(code: str) -> str:
    return "weather-" + ICON_FOR_WWO[WWO_CODE[code]]


def get_lang() -> str:
    default_locale = locale.getdefaultlocale()[0]
    if "_" not in default_locale:
        return "en"
    return default_locale.split("_")[0]


def get_base_url(city: str) -> str:
    lang = get_lang()
    return f"https://wttr.in/{city}?lang={lang}"


def do_status(city: str):
    lang = get_lang()
    wttr_url = get_base_url(city) + "&format=j1"
    response = urllib.request.urlopen(wttr_url)
    dct = json.loads(response.read())
    data = dct["current_condition"][0]

    code = data["weatherCode"]
    icon = icon_for_code(code)

    # If a translated entry exists, it's called "lang_$LANG". If it's not
    # there, fall back to "weatherDesc".
    desc_obj = data.get(f"lang_{lang}", "weatherDesc")
    desc = desc_obj[0]["value"]

    temp_c = data["temp_C"]
    tooltip = f"{temp_c}°C, {desc}"

    out = {
        "iconName": icon,
        "toolTipText": tooltip,
        "refresh": 3_600,
    }
    print(json.dumps(out))


def do_activate(city: str):
    webbrowser.open_new_tab(get_base_url(city))


def main():
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=__doc__)

    parser.add_argument("--activate")
    parser.add_argument("--status", action="store_true")
    parser.add_argument("city", nargs="?", default="",
                        help="Name of the city to show weather for")

    args = parser.parse_args()

    if args.status:
        return do_status(args.city)
    if args.activate == "primary":
        return do_activate(args.city)

    return 0


if __name__ == "__main__":
    sys.exit(main())
# vi: ts=4 sw=4 et
