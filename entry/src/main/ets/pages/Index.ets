import { hilog } from '@kit.PerformanceAnalysisKit';

import { common, Want } from '@kit.AbilityKit';
import { vpnExtension, http } from '@kit.NetworkKit';

let context = getContext(this) as common.VpnExtensionContext;
let want: Want = {
  deviceId: "",
  bundleName: "com.example.testproject",
  abilityName: "MyVpnExtAbility",
};


class Header {
  public contentType: string;

  constructor(contentType: string) {
    this.contentType = contentType;
  }
}




@Entry
@Component
struct Index {
  @State message: string = 'Hello World'

  build() {
    Row() {
      Column() {
        Text(this.message)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            console.info("Text clicked");
          })

        // Start VPN Button
        Button('Start VPN').onClick(() => {
          console.log("Starting VPN");
          vpnExtension.startVpnExtensionAbility(want);

        }).width('70%').fontSize(45).margin(16)

        // Close VPN Button
        Button('Close VPN').onClick(() => {
          console.log("Closing VPN");
          vpnExtension.stopVpnExtensionAbility(want);
        }).width('70%').fontSize(45).margin(16).backgroundColor(Color.Red)

        // Close VPN Button
        Button('发送请求').onClick(() => {
          console.log("发送请求");

          let httpRequest = http.createHttp();
          let options: http.HttpRequestOptions = {
            method: http.RequestMethod.POST, // 可选，默认为http.RequestMethod.GET
            // 当使用POST请求时此字段用于传递请求体内容，具体格式与服务端协商确定
            extraData: 'hello world',
            expectDataType: http.HttpDataType.STRING, // 可选，指定返回数据的类型
            usingCache: true, // 可选，默认为true
            priority: 1, // 可选，默认为1
            // 开发者根据自身业务需要添加header字段
            header: new Header('application/json'),
            readTimeout: 60000, // 可选，默认为60000ms
            connectTimeout: 60000, // 可选，默认为60000ms
            usingProtocol: http.HttpProtocol.HTTP1_1, // 可选，协议类型默认值由系统自动指定
            usingProxy: false, //可选，默认不使用网络代理，自API 10开始支持该属性
          };

          httpRequest.request("http://127.0.0.1:12345", options, (err: Error, data: http.HttpResponse) => {
            if (!err) {
              console.info('Result:' + data.result);
              console.info('code:' + data.responseCode);
              console.info('type:' + JSON.stringify(data.resultType));
              console.info('header:' + JSON.stringify(data.header));
              console.info('cookies:' + data.cookies); // 自API version 8开始支持cookie
            } else {
              console.info('error:' + JSON.stringify(err));
            }
          })



        }).width('70%').fontSize(45).margin(16).backgroundColor(Color.Red)

      }.width('100%')
    }.height('100%')
  }
}