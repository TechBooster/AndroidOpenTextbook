= あめ玉帝国のやばう

なまそでは激怒した。

あめ玉帝国という名の元に招集したすべてのなまそでが激怒した。

なまそでは四人衆の中でも最弱である。なぜなら袖が濡れているからである。

なまがわきである。

四人衆で最も偉大なのは半袖である。次は長袖である。三番目は五分袖である。

七分袖はなまそでに殺され、かつてその長男のユリウス・シチブソデは離島に追いやられた。
しかしシチブソデ

== これまでの章で説明していて欲しいこと

* TCP/IP, および下層のレイヤ

== 説明する予定のこと

本章では、Android端末の外側の世界にあたる、
Webサーバと通信する際に役に立つ基礎知識を説明します。

* RFC
* HTTP / HTTPS
* Authentication / Authorization
* REST / RESTful
* XML, JSONの概要
* HttpURLConnection を試す
* おまけ: http://y-anz-m.blogspot.jp/2011/10/androidapache-http-client.html

= RFC

Request For Comment の略です

= HTTP とは

HTTPには0.9, 1.0, 1.1があります。
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
RFC 7230に関わっているMark Nottingham氏は自身のブログ記事"RFC2616 is Dead"（RFC2616 は死んだ）@<fn>{mark_nottingham}で、
「RFC2616は使わないでください。ハードドライブ、ブックマークから削除し、
印刷されているなら全て燃やしてしまうかリサイクルに回しましょう」とまで書いています。 
HTTP 1.1について深く調べる際には、RFCを参照することはもちろんですが、関連記事を読む際に、古いRFCを元にしていないかを気にするのは良いことでしょう。

//footnote[mark_nottingham][@<href>{https://www.mnot.net/blog/2014/06/07/rfc2616_is_dead}]

