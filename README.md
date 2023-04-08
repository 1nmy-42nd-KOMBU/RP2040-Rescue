# Picoをぶっ壊す！
僕が大会前日に手持ちのPico2台を破壊した方法を教えるよ！  
I2Cで既定以外のピンを使うのにsetSDA,setSCLを使うけど、ここでピンの設定をミスって物理的に問題のあるコードを書き込むとUSBポートが破壊されるよ！[参考](https://www.toolsbox.biz/nonsense/RP2040/repairSerialCOM.html)

# 備忘録
* PlatformIOで使うときの手順 https://aloseed.com/it/platformio_ide_rp2040_memo/
* PlatformIOだと書き込むたびにBOOTSELを押しながら再接続しないといけない
