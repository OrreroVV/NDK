import { hilog } from '@kit.PerformanceAnalysisKit';
import testNapi from 'libccn.so';

import { common, Want } from '@kit.AbilityKit';
import { vpnExtension, http } from '@kit.NetworkKit';

let context = getContext(this) as common.VpnExtensionContext;
let want: Want = {
  deviceId: "",
  bundleName: "com.example.testproject",
  abilityName: "MyVpnExtAbility",
};
@Entry
@Component
struct Index {
  @State message: string = 'Hello World';

  build() {
    Row() {
      Column() {
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


      }
      .width('100%')
    }
    .height('100%')
  }
}
