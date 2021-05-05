# これはロボットキット用のプログラムです。

プログラムはテンプレートに名前をつけて保存してから使ってください。

1. dist();
- 超音波センサの値を返します。

1. motor(spd1,spd2,spd3);
- モーターの制御を行います。spd1,spd2,spd3はそれぞれモーター1,2,3のスピードを表しています。-255〜255までの値が有効です。マイナスは反対方向に回ります。

1. getLine(int num);
- ラインセンサの値を取得します。取得したいラインセンサの番号を入力してください。ラインセンサはロボットの前から時計回りに1~4までの番号が割り振られています。

1. getIr(int num);
- 赤外線センサの値を返します。取得したい赤外線センサの番号を入力してください。赤外線センサはロボットの前から時計回りに1~4までの番号が割り振られています。

1. irUpdate();
- 赤外線センサの値を更新します。ループ毎に一度読み込んでください。

1. getFlontAzim();
- 電源スイッチを押した時点での機体の方位を返します。

1. getAzim();
- 現在の方位を出力します。

1. initialize();
- ピンやi2c、シリアルポートなどの初期化を行います。必ずsetup()内に記述してください。

1. robot(int minX, int maxX, int minY, int maxY);
- QMC5883Lライブラリのcalibtationプログラムを実行します。
- setCalibration(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max);を取得します。
- int x_min, int x_max, int y_min, int y_maxの部分をコピーしてint minX, int maxX, int minY, int maxYの部分に貼り付けます。
- 終わり！！

