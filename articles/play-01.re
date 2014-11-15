= Google Playにアプリを公開する

本章では、アプリをGoogle Playに公開する手順を解説します。Google Playは、Googleが提供するアプリストアです。
開発者は、Google Playストアを通じて利用者にアプリを提供できるほか、Developer Consoleからアプリケーションのアップデートやエラーレポートなど
開発に有用なフィードバックを受け取れます。

== この章で学ぶこと

 * Google Play デベロッパー契約の手順
 * Developer Consoleでのアプリ公開手順
 * アプリケーションへの署名方法

この章で出てくるキーワード一覧

Google Play、Developer Console、電子署名、keystore

== 開発者として登録する
Google Playストアでアプリを公開するためには、有料の開発者登録が必要です。

 * https://play.google.com/apps/publish/

登録はDeveloper Consoleで行います（@<img>{Google_Play_Developer_Console_1}）。

//image[Google_Play_Developer_Console_1][Developer Console]{
//}

登録には、GoogleアカウントとGoogleウォレットを使い、登録料として25ドルが請求されます。
GoogleアカウントとGoogleウォレットに追加するクレジットカードを用意します。
登録料の支払いは通常のクレジットカード、デビッドカードはもちろん、コンビニ等で入手できるプリペイド式のクレジットカードも利用できます。

Google Playデベロッパー販売/配布契約書に同意して、Googleウォレットの設定に進みます（@<img>{Google_Play_Developer_Console_2_pay}）。

//image[Google_Play_Developer_Console_2_pay][Googleウォレットの設定]{
//}

Googleウォレットでは、氏名、カード番号、有効期限を入力して、購入内容の確認画面へ進みます（@<img>{Google_Play_Developer_Console_3_paid}）。


//image[Google_Play_Developer_Console_3_paid][ご購入内容の確認]{
//}

購入画面では、支払い先、商品金額（$25.00USD）に間違いがないことを確認して購入ボタンを押してください。
支払い情報の入力に進みます（@<img>{Google_Play_Developer_Console_4}）。

//image[Google_Play_Developer_Console_4][お支払い情報の更新]{
//}

支払い情報では、住所、電話番号など請求先の情報を入力します。購入ボタンを押せば、決済が確定します。購入処理ダイアログが表示されたあと、
デベロッパープロフィールを入力します（@<img>{Google_Play_Developer_Console_6}）。

//image[Google_Play_Developer_Console_6][デベロッパープロフィール]{
//}

開発者として登録する内容を入力します。デベロッパー名、メールアドレスはアプリを公開する際に使用されます。
登録を終了すると、Developer Consoleが表示されます（@<img>{Google_Play_Developer_Console_7}）。

//image[Google_Play_Developer_Console_7][Developer Console]{
//}

Developer Consoleでは、公開するアプリケーションを追加できるほか、
アプリ内課金、開発をサポートするチーム機能、ゲーム機能を追加するゲームサービスなどが用意されています。


== アプリを公開する
Google Playストアでアプリを公開する手順を解説します。アプリタイトルや説明文、スクリーンショットを事前に用意してください。
公開するアプリケーションの本体ファイル（APKファイル）には、電子署名が必要です（署名の手順は次の節で解説します）。

「Google PlayにAndroidアプリを公開」ボタンを押して、ウィザードを始めましょう（@<img>{Google_Play_Developer_Console_11}）。

//image[Google_Play_Developer_Console_11][Google PlayにAndroidアプリを公開]{
//}

ウィザードでは、言語とタイトルを入力します（@<img>{Google_Play_Developer_Console_12}）。

//image[Google_Play_Developer_Console_12][新しいアプリを追加]{
//}

タイトルは30文字以内です。入力後、「APKをアップロード」を選択してアプリケーションをアップロードしましょう（@<img>{Google_Play_Developer_Console_13}）。

//image[Google_Play_Developer_Console_13][APKのアップロード]{
//}

APKのアップロード画面では、製品版、ベータ版テスト、アルファ版テストの３つのタブがあります。公開時は製品版としてAPKファイルをアップロードします。
ベータ版テストやアルファ版テストは、テストユーザーに限定して配布できます。特定のユーザーを登録して配布できるため、公開前の検証に便利です。

次の項目「ストアの掲載情報」では、アプリケーションの説明を追加します（@<img>{Google_Play_Developer_Console_14}）。

//image[Google_Play_Developer_Console_14][ストアの掲載情報 - 説明文]{
//}

ここで入力したアプリケーションの説明文は、Google Playストアの利用者が閲覧します。入力項目は次の通りです。

 * タイトル（30字）
 * 簡単な説明（80文字）
 * 詳細な説明（4000文字）
 
それぞれ括弧内が最大文字数です。デフォルトの言語で入力する以外にも、翻訳を追加できます。まずは日本語からはじめて英語など多言語対応するとよいでしょう。

//image[Google_Play_Developer_Console_14_2][ストアの掲載情報 - 画像アセット]{
//}

説明文の次は、画像アセットの登録です（@<img>{Google_Play_Developer_Console_14_2}）。少なくとも2つのスクリーンショット（最大8つ）をアップロードします。
電話、7インチタブレット、10インチタブレット、テレビ向けそれぞれで追加できます。画面サイズに応じてレイアウトが違う場合は積極的に活用してください。
Google Playストアで、タブレット向けのアプリケーションとしてリストに掲載されるなど掲載箇所が増える利点があります。

//image[Google_Play_Developer_Console_14_3][ストアの掲載情報 - アイコンと宣伝用画像]{
//}

@<img>{Google_Play_Developer_Console_14_3}では、プロモーションを目的として、高解像度アイコン、宣伝用画像、バナー、動画を登録します。
バナー、動画はオプションですが、アプリの特徴をわかりやすく伝え、利用者の目にも留まる効果があります。


//image[Google_Play_Developer_Console_14_4][ストアの掲載情報 - 分類と連絡先]{
//}

「ストアの掲載情報」の最後、分類と連絡先（@<img>{Google_Play_Developer_Console_14_4}）では以下の項目を埋める必要があります。

 * アプリのタイプ：アプリケーション、ゲームの２つから選択
 * カテゴリ：Google Playストアのアプリケーションジャンルから選択する
 * コンテンツレーティング：アプリの対象者を成熟度でフィルタリングする

次の連絡先情報は、アプリの紹介ウェブサイト、問い合わせ先となるメールアドレス、電話番号を記入します。
またアプリで利用者情報を扱う場合には、プライバシーポリシーを用意して、リンクするURLを指定します。

最後に「価格と販売/配布地域」で配布するアプリケーションの配布対象、値段を指定します。国単位で配布する地域を選べます。

アプリを公開する準備が整うと、画面左端の「APK」「ストアの掲載情報」「価格と販売/配布地域」すべてにチェックマークがつきます。
最後に画面右端のプルダウンを「ドラフト」から「公開する」に変更して完了です（@<img>{Google_Play_Developer_Console_15}）。

//image[Google_Play_Developer_Console_15][ストアの掲載情報 - アプリを公開する]{
//}

公開後、APKファイルのウイルスチェックが行われるため、実際にGoogle Playストアに反映されるまでは数時間から半日かかります。

== アプリに署名する
Google Playストアにアプリを公開する場合、なりすましや偽造防止のためにAPKファイルへの電子署名が必要です。
電子署名されていないAPKファイルはDeveloper Consoleへアップロードできません。
この節では、EclipseのAndroidアプリプロジェクトから電子署名つきのAPKファイルを作成します。


//image[keystore_1][プロジェクトのエクスポート]{
//}

Eclipseで電子署名したいプロジェクトを右クリックして[Export...]を選択します（@<img>{keystore_1}）。

//image[keystore_2][エクスポートウィザード]{
//}

Export形式を確認するウィザードが起動します（@<img>{keystore_2}）。ここではAndroidの「Export Android Application」を選択します。


//image[keystore_3][プロジェクト名の確認]{
//}

署名したいプロジェクトを確認します（@<img>{keystore_3}）。問題なければNextを押して進みます。


//image[keystore_5][キーストアの生成]{
//}


電子署名するにはkeystore（キーストア）が必要です（@<img>{keystore_5}）。初回であれば「Create new keystore」を選択してください。ここで作成したキーストアは保管しておき、アプリの更新時に再利用します。
一度公開したアプリを更新であれば「Use existing keystore」を選び、保管しておいたキーストアを使い、署名します。
もしキーストアを紛失してしまうと、Google Playストアでは同じアプリだと認識できなくなり、開発者であってもアプリの更新が行えなくなりますので注意して管理します。

//image[keystore_6][キーの詳細情報を入力]{
//}

キーストアに保管するキー（鍵）の詳細を入力します（@<img>{keystore_6}）。鍵の名前（Alias）、パスワード（ここのPasswordは先ほどのKeystoreのパスワードとは異なる文字列を指定できます）、
鍵の有効期限（Gooogle Playストアで公開するには30年以上の有効期限が必要）を記入します。続いて入力する所有者情報も電子署名の一部として参照できるため公開されても問題のない範囲で記入してください。



//image[keystore_7][電子署名したAPKファイルを出力する]{
//}

ウィザードの最後で電子署名したAPKファイルの出力先を選択し、完了です（@<img>{keystore_7}）。

==[column]
Developer Consoleでは、電子署名されたAPKファイルのみアップロードを受け付けています。2回目以降は、前回と同じ署名を使わないと、同一のアプリとみなされません。
キーストアファイルを紛失しないように十分気を付けてください。またセキュリティの観点からは、アプリケーションごとに異なる鍵を使って署名することで強度を保てます。
==[/column]