= あめ玉帝国のやばう

なまそでは激怒した。

あめ玉帝国という名の元に招集したすべてのなまそでが激怒した。

なまそでは四人衆の中でも最弱である。なぜなら袖が濡れているからである。

なまがわきである。

四人衆で最も偉大なのは半袖である。次は長袖である。三番目は五分袖である。

七分袖はなまそでに殺され、かつてその長男のユリウス・シチブソデは離島に追いやられた。
しかしシチブソデ


== 説明する予定のこと

本章では、Android端末の外側の世界にあたる、Webサーバと通信する際に役に立つ基礎知識を説明します。

 * サーバの役割
 * RFC
 * HTTP / HTTPS
 * Authentication / Authorization
 * REST / RESTful
 * XML, JSONの概要
 * HttpURLConnection を試す


== サーバの役割

サーバとはそもそも何でしょうか。

== サーバ、クライアント

英語で言えばserverで、日本語でも例えば
「ジュースサーバ」「ビールサーバ」といった表現で登場します。

//image[juiceserver][ジュースサーバー]{
//}

英語名詞であるserverに対応する動詞（「〜する」）はserveです。
これは「〜に仕える」「〜のために働く」という意味です。
serverはserveする人、という意味で、
コンピュータ関連の用語ではない本来の意味は「給仕人」「接客係」です。
つまり、仕える・給仕する相手が前提にある表現です。@<fn>{kyuuji_nin}

//footnote[kyuuji_nin][ちなみに給仕の読みは「きゅうじ」であって「きゅうし」ではありません。]

コンピュータ関連用語では、サーバが仕える相手を「クライアント」と呼びます。

こちらの元の英語はclientです。
日本語でも、お客さんを「クライント」と呼ぶ分野がありますね。



 * データを保存・共有するために必要
 * BaaS
 * Webブラウザから見える「独立のアプリケーション」
 * SaaS
 * AWS


== RFCとは

Request For Comment の略です

== HTTP とは

執筆時点ではHTTPには0.9, 1.0, 1.1があります。
現在2.0の仕様策定中ですが、完成はしていません。

HTTP 0.9が極めてシンプルだったものであったのに対して、
HTTP 1.1の仕様にあたるRFC 2616では実質176ページ
(TODO: 要根拠)に渡る長大なものとなっています。

HTTP 1.1は1999年6月に公開されたRFC 2616ですが、
約15年経った今では様々な課題を抱えています。
特に当時は想定されていなかったWebブラウジング以外での
HTTPの利用が増えたことが大きいでしょう。
(TODO: AJAX？)

== HTTP 1.1 の新RFCについて

そこで、2014年6月に、このHTTP 1.1の仕様が
15年ぶりに改定されました。現在ではその内容が
下記6つのRFCとして公開されています。

* RFC7230: Message Syntax and Routing
* RFC7231: Semantics and Content
* RFC7232: Conditional Requests
* RFC7233: Range Requests
* RFC7234: Caching
* RFC7235: Authentication

(TODO: 概要説明)

この改定では、HTTPのバージョンが増えたわけではありません。
HTTP 1.1と呼ぶとき、今後はRFC 2616ではなく上記のRFCのみを参照するようにしてください。
ちなみに、
RFC 7230に関わっているMark Nottingham氏は自身のブログ記事"RFC2616 is Dead"（RFC2616 は死んだ）
@<fn>{mark_nottingham}で、
「RFC2616は使わないでください。ハードドライブ、ブックマークから削除し、
印刷されているなら全て燃やしてしまうかリサイクルに回しましょう」とまで書いています。 
HTTP 1.1について深く調べる際には、RFCを参照することはもちろんですが、関連記事を読む際に、古いRFCを元にしていないかを気にするのは良いことでしょう。

//footnote[mark_nottingham][@<href>{https://www.mnot.net/blog/2014/06/07/rfc2616_is_dead}]


== HTTP 2.0 について

HTTP 2.0はHTTP 1.1の次バージョンとして標準化が行われている次のHTTP仕様です。
Googleが開発しているSPDFと呼ばれる仕組みを元にしています。

仕様策定としては最終段階にありますが、
「そもそもこれを公式の仕様とするべきではない」


http://www.phoronix.com/scan.php?page=news_item&px=MTcwMTA


== Android アプリからWebサーバからデータを取得する

実際にAndroid経由でHTTPアクセスをしてみましょう。
ここでは@<code>{java.net.HttpURLConnection}を用いることにします。

===[column] DefaultHttpClientとAndroidHttpClient について

HTTPアクセスを行うライブラリは他にも@<code>{org.apache.http.impl.client.DefaultHttpClient}や
@<code>{android.net.http.AndroidHttpClient}といったライブラリが紹介されることがあります。
しかし本稿ではそれらの理由はおすすめしません。
現時点でこれらの実装にはバグが多く、今後もメンテナンスされる見込みがないと考えられているためです。
なお、非常に古いAndroidのバージョン（2.2）では@<code>{java.net.HttpURLConnection}にバグがあったため、
それらの古いバージョンでは注意が必要です。

 * 参考: Android Apache HTTP Client と HttpURLConnection どっちを使うべき？ @<href>{http://y-anz-m.blogspot.jp/2011/10/androidapache-http-client.html}@<fn>{this_is_also_old_article}


//footnote[this_is_also_old_article][記事の公開2011年10月4日に公開された点も出来れば]



== UrlHttps を試す
== SSL

== 認証・認可について

 * 認証: 本人確認
 * 認可: アクセス許可

コンピュータの世界から離れて役所で公的な証明書を受け取ることを考えてみます。
例えば、戸籍。

申請書類を書いて、免許証と申請書類を提出し、
役所が大丈夫だと思えば、役所が持っている情報を提供してくれます。

免許証を見せることは自分が本人であることの確認です。
もし「自分である」ことがわかれば、方法は複数あります。


しかし、見せたからと言って、申請する書類の許可が常に得られるとは限りません。

