# SpumoNI

Easily create StatusNotifierItem apps using any language.

## Usage

    spumoni <a spumoni command>

## SpumoNI "protocol"

### --status

```json
{
    "iconName": "",
    "text": "",
    "refresh": int // interval in seconds, 0 = no refresh
    "actions": [
        {
            "id": "",
            "text": "",
            "iconName": ""
        }
    ]
}
```

### --activate (primary|secondary|increase|decrease)

### --activate-action <id>

## Building it

This application requires Qt 5 and CMake. To build it, do the following:

Get the source:

    git clone https://github.com.com/agateau/spumoni
    cd spumoni

Build it:

    mkdir build
    cd build
    cmake ..
    make
    sudo make install

You can also build rpm and deb files using `make package` after `make`.


## License

Released under GPL v3.
