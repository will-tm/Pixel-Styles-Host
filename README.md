# Pixel Styles

Pixel Styles (host part) is an embedded linux daemon that intend to provide 1D and 2D visualizations for WS2801 based RGB matrix or strips.

[![build status](http://ci.will-tm.com/projects/2/status.png?ref=develop)](http://ci.will-tm.com/projects/2?ref=develop)

![Screenshot](http://download.will-tm.com/spec.png)

## Requirements
* Embedded armhf linux board (tested on Raspberry Pi, Beaglebone Black and Cubieboard - all running Debian Wheezy)
* libboost
* libmuduo
* libjson_spirit
* libbass
* Network connection (for client application)
* spidev

## Installation

### From my repository
``` bash
echo 'deb http://repo.will-tm.com/ stable main' >> /etc/apt/sources.list
wget -q -O - http://repo.will-tm.com/key.pub | apt-key add -
apt-get update
apt-get install pixel-styles -y

```

### From sources
``` bash
make all
make install
```

## Configuration

Edit `/etc/pixel_styles/config.cfg` and provide the path to you spi device and your maxtrix size.

You will have to restart the daemon `service pixel-styles restart` for your settings to be loaded. 

## Add modes

You can create your own visualization modes by adding then in `./src/modes/lib`, using the name `mode_xxx.*` then run `./tools/build_mode xxx`.

The compiled output will be available at `./output/lib/xxx.so`.

Upload the file to you device at `/etc/pixel_styles/modes-available/` then run psenmod xxx to enable your mode.
Finally you will have to restart the daemon `service pixel-styles restart` and your mode will be loaded.

## Authors

* William Markezana ([@Will_tm](https://twitter.com/Will_tm))

## License

Pixel Styles is available under the MIT license.

Copyright © 2014 William Markezana.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
