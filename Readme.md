#libprefmpd
The aim of this library is to make your MPD client automatically use your
preferred MPD host (thus the name) when it is available.

## Usage
Add the absolute path of `libprefmpd.so` to `LD_PRELOAD`.
Export `PREF_MPD_PORT` and `PREF_MPD_HOST` to point to your preferred MPD host.

## How does it work?
`libprefmpd` hijacks `getenv()` and generates results on the fly for queries of
`MPD_HOST` and `MPD_PORT`.
The presence of your preferred MPD host is determined by looking up its name.
