import { hilog } from '@kit.PerformanceAnalysisKit';

import { getUrl } from 'libentry.so';

@Entry
@Component
struct Index {
  @State message: string = 'Hello World';

  build() {
    Row() {
      Column() {
        // 显示文本
        Text(this.message)
          .fontSize(50)
          .fontWeight(FontWeight.Bold)
          .onClick(() => {
            hilog.info(0x0000, 'testTag', 'Test URL: %{public}s', getUrl("https://192.168.20.141/_/rweb/realms"));
          })
        Button('test socket')
          .fontSize(30)
          .onClick(() => {
            getUrl("https://192.168.20.141/_/rweb/realms");
            // hilog.info(0x0000, 'testTag', 'Test URL: %{public}s', testNapi.getUrl("https://192.168.20.141/_/rweb/realms"));
          })
          .width('100%')
        // 按钮组件
        Button('Click Me')
          .fontSize(30)
          .onClick(() => {
            // hilog.info(0x0000, 'buttonTag', 'Test NAPI 5 + 7 = %{public}d', add(5, 7));
          })
          .width('100%')
      }
      .width('100%')
    }
    .height('100%')
  }
}
