import { vpnExtension, VpnExtensionAbility, connection } from '@kit.NetworkKit';
import { common, Want } from '@kit.AbilityKit';
import { hilog } from '@kit.PerformanceAnalysisKit';
import { getUrl, native_setFd } from 'libentry.so';


export default class MyVpnExtAbility extends VpnExtensionAbility {
  private tunIp: string = '10.0.0.5';
  private blockedAppName: string = 'com.example.testproject';
  private tunFd: number = -1;
  onCreate(want: Want) {
    type LinkAddress = connection.LinkAddress;
    console.info("start onCreate");
    let VpnConnection : vpnExtension.VpnConnection = vpnExtension.createVpnConnection(this.context as common.VpnExtensionContext);
    console.info("vpn createVpnConnection: " + JSON.stringify(VpnConnection));
    this.SetupVpn();
  }
  onDestroy() {
    console.log('MyVpnExtAbility onDestroy');
  }

  SetupVpn() {
    class Address {
      address: string;
      family: number;

      constructor(address: string, family: number) {
        this.address = address;
        this.family = family;
      }
    }

    class AddressWithPrefix {
      address: Address;
      prefixLength: number;

      constructor(address: Address, prefixLength: number) {
        this.address = address;
        this.prefixLength = prefixLength;
      }
    }



    class Config {
      addresses: AddressWithPrefix[];
      mtu: number;
      dnsAddresses: string[];
      trustedApplications: string[];
      blockedApplications: string[];

      constructor(
        tunIp: string,
        blockedAppName: string
      ) {
        this.addresses = [
          new AddressWithPrefix(new Address(tunIp, 1), 24)
        ];
        this.mtu = 1400;
        this.dnsAddresses = ["114.114.114.114"];
        this.trustedApplications = [];
        this.blockedApplications = [blockedAppName];
      }
    }

    let config = new Config(this.tunIp, this.blockedAppName);




    try {
      hilog.error(0x0000, 'developTag', 'start vpn connect');
      let VpnConnection : vpnExtension.VpnConnection = vpnExtension.createVpnConnection(this.context);
      VpnConnection.create(config).then((data) => {
        this.tunFd = data;
        hilog.error(0x0000, 'developTag', 'tunfd: %{public}s', JSON.stringify(data) ?? '');
        native_setFd(this.tunFd);
      })
    } catch (error) {
      hilog.error(0x0000, 'developTag', 'vpn setUp fail: %{public}s', JSON.stringify(error) ?? '');
    }
  }
}