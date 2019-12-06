# ChirpSDK for Arduino Changelog

Recent changes to the [Chirp Arduino SDK](https://developers.chirp.io/docs).

## v3.4.1 (beta)
 - Add support for Teensy boards (cortex-m4 hard float build)

## v3.4.0 (18/09/2019)
 - All functions have been renamed to `chirp_sdk*` from `chirp_connect*`
 - `chirp_sdk_get_heap_usage` has been added. This gives you the current dynamic memory allocated and used at any time by the SDK.
 - `chirp_connect_pause` has been removed.
 - `chirp_connect_as_string` has been removed. You can find alternatives on our [Payload](https://developers.chirp.io/docs/using-chirp/payloads) page.
 - `chirp_sdk_(set/get)_auto_mute` becomes `chirp_sdk_(set/get)_listen_to_self`. Be aware these are opposite meanings so don't forget to switch `true` by `false` and vice versa when updating your code.
 - `CHIRP_SDK_MEMORY_LEAK` error code has been added and is returned by `del_chirp_sdk` when some memory leaks occur when deleting the SDK.
 - `CHIRP_SDK_RECEIVING_NOT_AVAILABLE` error code has been added and is returned when trying to use decoding features with a library which is send only.
 - Core build v3.3.1

## v3.3.1 (22/08/2019)
 - Use high frequency oscillator in Nano 33 Sense examples

## v3.3.0 (09/08/2019)

 - Added support for cortex-m4 (Nano 33 Sense)
 - Added send-only support for cortex-m0plus (MKRZero, MKR Vidor 4000)
 - Build v3.3.0-rc1

## v3.2.0 (14/03/2019)

 - Build v3.2.9
 - Optimised DSP for ESP32

## v3.1.0 (11/03/2019)

 - Build v3.2.8
 - Significant memory improvements

## v3.0.0 (03/03/2019)

 - Build v3.2.5
 - cortex-m4 architectures for MXChip
