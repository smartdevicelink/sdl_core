package com.ford.syncV4.protocol.secure.secureproxy;

import android.util.Base64;

import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.transport.ITransportListener;
import com.ford.syncV4.util.DebugTool;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.CertificateException;
import java.util.Arrays;

import javax.net.ssl.HandshakeCompletedListener;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLServerSocket;


/**
 * Created by Andrew Batutin on 3/20/14.
 */
public class SSLServer implements ISSLComponent {

    private final HandshakeCompletedListener handshakeCompletedListener;
    ITransportListener transportListener;
    private IRPCodedDataListener RPCPacketListener;
    private SSLServerReader sslServerReader;
    private Socket socket;

    public synchronized SSLServerSocket getServerSocket() {
        return serverSocket;
    }


    private SSLServerSocket serverSocket;


    public SSLServer(ITransportListener transportListener, HandshakeCompletedListener handshakeCompletedListener) {
        this.transportListener = transportListener;
        this.handshakeCompletedListener = handshakeCompletedListener;
    }

    @Override
    public void setupClient(int localPort) throws IOException {
        sslServerReader = new SSLServerReader();
        sslServerReader.start();
    }

    @Override
    public synchronized void writeData(byte[] data) throws IOException {
        socket.getOutputStream().write(data);
    }

    @Override
    public void setRPCPacketListener(IRPCodedDataListener RPCPacketListener) {
        this.RPCPacketListener = RPCPacketListener;
    }

    @Override
    public IRPCodedDataListener getRPCPacketListener() {
        return RPCPacketListener;
    }

    private class SSLServerReader extends Thread {

        public synchronized boolean isConnected() {
            return isConnected;
        }

        public synchronized void setConnected(boolean isConnected) {
            this.isConnected = isConnected;
        }

        private boolean isConnected;
        private final String KEYSTORE_BKS = "AAAAAgAAABS676E1WzKhMvohA0ox3oVSDukR2QAABTMEAAVteWtleQAAAUazs8xeAAAAAQAFWC41MDkAAAIhMIICHTCCAYagAwIBAgIEU6K9YzANBgkqhkiG9w0BAQUFADBTMQswCQYDVQQGEwJkZjEMMAoGA1UECBMDZHNmMQwwCgYDVQQHEwNkc2YxDDAKBgNVBAoTA3NkZjEMMAoGA1UECxMDc2RmMQwwCgYDVQQDEwNzZGYwHhcNMTQwNjE5MTAzNzIzWhcNMTQwOTE3MTAzNzIzWjBTMQswCQYDVQQGEwJkZjEMMAoGA1UECBMDZHNmMQwwCgYDVQQHEwNkc2YxDDAKBgNVBAoTA3NkZjEMMAoGA1UECxMDc2RmMQwwCgYDVQQDEwNzZGYwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAJGTQwknwSVJF5QNPV9nWNNFf09BNUAtwJ2UcFfbYDx3lwMDVA9sz7SADGgonBkQUliYH9DOEuj+SSCb2SJesrmQO/EVZBNkGcMtU34IVyAruKrCejL7AnkHAyhazWq+pb+4CRe5v38Uhcf1ieJSXdU1GOiVHrGqKEG2FSEXa0a1AgMBAAEwDQYJKoZIhvcNAQEFBQADgYEAJeFcbi7e/nVDRXsOlTHef7kRwi/GilbDJNog6J6/OaP1j77vrsVvK/usAsFYRHJBMNGlu2qOlp7q7VSCjI8WTF54Dl7KOvxrDLz3716UZxvIwRbPpmsq3/2yG0FusedRQx6MuzQkBsG0mzVZ6FGVK8p5UKTaJR24kwGPGT+vwnMAAAKsAAAAFJVIJxGpg0PXUfvFAkRibeAvpi9DAAAFBQhwxEjeiUI0yFCOk1lrNQ6w3Ey9xH6J6kuoB9eTQ/kwKrQ+dMFTbHd0dV8uZVY2Il0MjE1GhXrrEpfd6BzsuXrgRWOnPh/mkpkTNgo3BZgTyR4dWw5C7IRoMqOydeF5Y+m6vfBOnVw+L49HZXMvspVDI+6HR9JrUCcZxRZ2YG5DdGjrEhcdbCaHgIgFhT6SPWRF53fXAKGQvhYsFKfBoiUlSs96H9mXNB10sQZUo/8MfHrY/Fb4XlsGh1r+Vy4GOn0ydumVQ7PZLErMPxJQuoBM1ou+M+qI8Icug3INc+K1S6e9u9sGPEJdWnwkPJUcQd2wjgMoDH+hsQV0BxsXx6REwtRh6f0yp4+LBUeepZKrSeApyJxOCj1hVPld8FeaDrgf8s23qX9TvY5eS77y1YsTOrJI+FI84ZMb4vGN62bntH4w+TxB5epV2DD9a+jfVciwCQf7eGpDC7xcpj0wQqLt2ltwnG79bwwX+IpPEa2JYYW2gA8tKl4vEXDQC/77IJRgR7ZOnRT2Xn5vjjqaBvGtxriYx/+ixd3cbPEFNMZF+uRv+9XxP2PCFRumOX4Xv/7ZUilTSsb3njpCOqLc9zFJWwCCxIIS8P16AmJqkvatdibtu61qFVE75GQAsUd+Q9aGypLcQBJw9HlL9O32rtWjDjMg2gJbVKYlrDV7uxpsAdTzXOj9tDtZzOxEDyVlyCMVnaWVf1C35HmZHe94OAjY1kWfS4DmkivXSxoDuNx3kg8OsOAeWRCROaZDnwiWSDe1TQKdrgpm0OvKhLOJpGKX2BgcmfsbIvCq9UgzSYn80YHx9PQ+xqABwxoQJA7bD17GsXFKT1VTZHzJiYjSnR5GKI7oA9IGVXTguaw9WgD9ALORpzbijZ8zRE/5HtXQ4gSKHtCT";
        private final String CERTIFICATE_PEM = "QmFnIEF0dHJpYnV0ZXMKICAgIGxvY2FsS2V5SUQ6IDk4IERDIDUzIEVBIDUzIDIwIDNCIDYzIEM5IEM1IEM2IDEzIDFFIDhBIERCIDczIEU3IDU1IDM2IENBIApzdWJqZWN0PS9DPVJVL1NUPVNvbWUtU3RhdGUvTD1TYWludC1QZXRlcnNidXJnL089THV4b2Z0L0NOPURtaXRyeSBDaG1lcmV2L2VtYWlsQWRkcmVzcz1kY2htZXJldkBsdXhvZnQuY29tCmlzc3Vlcj0vQz1SVS9TVD1Tb21lLVN0YXRlL0w9U2FpbnQtUGV0ZXJzYnVyZy9PPUx1eG9mdC9DTj1EbWl0cnkgQ2htZXJldi9lbWFpbEFkZHJlc3M9ZGNobWVyZXZAbHV4b2Z0LmNvbQotLS0tLUJFR0lOIENFUlRJRklDQVRFLS0tLS0KTUlJRGxEQ0NBbndDQ1FESWU3QXdGcFdSbERBTkJna3Foa2lHOXcwQkFRVUZBRENCaXpFTE1Ba0dBMVVFQmhNQwpVbFV4RXpBUkJnTlZCQWdNQ2xOdmJXVXRVM1JoZEdVeEdUQVhCZ05WQkFjTUVGTmhhVzUwTFZCbGRHVnljMkoxCmNtY3hEekFOQmdOVkJBb01Ca3gxZUc5bWRERVhNQlVHQTFVRUF3d09SRzFwZEhKNUlFTm9iV1Z5WlhZeElqQWcKQmdrcWhraUc5dzBCQ1FFV0UyUmphRzFsY21WMlFHeDFlRzltZEM1amIyMHdIaGNOTVRRd01qSTFNRGt4T0RVegpXaGNOTVRZeE1USXlNRGt4T0RVeldqQ0JpekVMTUFrR0ExVUVCaE1DVWxVeEV6QVJCZ05WQkFnTUNsTnZiV1V0ClUzUmhkR1V4R1RBWEJnTlZCQWNNRUZOaGFXNTBMVkJsZEdWeWMySjFjbWN4RHpBTkJnTlZCQW9NQmt4MWVHOW0KZERFWE1CVUdBMVVFQXd3T1JHMXBkSEo1SUVOb2JXVnlaWFl4SWpBZ0Jna3Foa2lHOXcwQkNRRVdFMlJqYUcxbApjbVYyUUd4MWVHOW1kQzVqYjIwd2dnRWlNQTBHQ1NxR1NJYjNEUUVCQVFVQUE0SUJEd0F3Z2dFS0FvSUJBUUNuCk11OXdObVhLYUtPbmVmU3Y1aVQ4RzJFU0xqcStlWWx4eXMvWEFuRGZrbW5sZ0dZQWNQbm8rWE1oUmovbE5WL2MKM0EwTC9SNDYzMUdGSkE4dmFNOG05Qm40N0ZyUFA0QVhJSEVRaDlhY0E0cVhpTGZoaEE4KzlQUHQ0eFZralFZagpibWV4QkxxRHZSZ1QzTUp3d0ZlY1VuL1VBQkJsVlpSQ3Nwbis2RGtqaW9kYmdtQk9xeWkxcDBuZzhCRmVVYkVICitmTFFWSUxDWDNwam5NaVAyYkJ0dnEvN25qZ1pUMmx1VnRBQWNPZFJ3UlR1WlQwWWJnYVhySFlzT2E2VllEbDEKSTB1T2NkRDhxRU5CWHRCbnlrRXFIK2padEt1NlJlajFEc0dPWVdxejNBQWFHaVIxR0phdU5CeGgrNHYraS9lQgowYUNJQThUOHFVcXl1VlZnNDhTL0FnTUJBQUV3RFFZSktvWklodmNOQVFFRkJRQURnZ0VCQUNsaXJhT0pZaWpLCnlTK1NsNlM2cEZScWRGL2V2UGRZRjZ6REpsTTNQKy85cUhvRXk3NTF2YkJUelJrVmJDL2F6eWlaTHdRTXV5RUQKNm9DcGtJN01xbnJSaXAxWmVsR3g5SzdDaGFIT3BYL1FSTiszZXFpRGh6dk1UR2QyblBKZjlucDR4aThTSnBHUApVVVJPWUk1ZlRvSVk1TWFPS3VPSVIyYTZjOHhJdUxXTUcxWEtKeFhyUmV0TEpaRGdCcVFQa3VxYVpJallDWStxCkhRUmpOVUZOWDRNYzQ1M3RLZDkwZ0ZMR0kzZnhzMWZKRElSU0dmS0pzajBxYythbVN6NFNnaXo0UVVCY1VRS2QKaEp4VXBTdFlobGlaR1pjaEVvcExzU2h0SUdmS0tGYWFQQ0lPVHBWQXdTcjFvSURtOWxwa2R4ZXVRZmVkS1Q1ZgpaWm1rZXoycEFGOD0KLS0tLS1FTkQgQ0VSVElGSUNBVEUtLS0tLQpCYWcgQXR0cmlidXRlcwogICAgbG9jYWxLZXlJRDogOTggREMgNTMgRUEgNTMgMjAgM0IgNjMgQzkgQzUgQzYgMTMgMUUgOEEgREIgNzMgRTcgNTUgMzYgQ0EgCktleSBBdHRyaWJ1dGVzOiA8Tm8gQXR0cmlidXRlcz4KLS0tLS1CRUdJTiBSU0EgUFJJVkFURSBLRVktLS0tLQpNSUlFb3dJQkFBS0NBUUVBcHpMdmNEWmx5bWlqcDNuMHIrWWsvQnRoRWk0NnZubUpjY3JQMXdKdzM1SnA1WUJtCkFIRDU2UGx6SVVZLzVUVmYzTndOQy8wZU90OVJoU1FQTDJqUEp2UVorT3hhenorQUZ5QnhFSWZXbkFPS2w0aTMKNFlRUFB2VHo3ZU1WWkkwR0kyNW5zUVM2ZzcwWUU5ekNjTUJYbkZKLzFBQVFaVldVUXJLWi91ZzVJNHFIVzRKZwpUcXNvdGFkSjRQQVJYbEd4Qi9ueTBGU0N3bDk2WTV6SWo5bXdiYjZ2KzU0NEdVOXBibGJRQUhEblVjRVU3bVU5CkdHNEdsNngyTERtdWxXQTVkU05Mam5IUS9LaERRVjdRWjhwQktoL28yYlNydWtYbzlRN0JqbUZxczl3QUdob2sKZFJpV3JqUWNZZnVML292M2dkR2dpQVBFL0tsS3NybFZZT1BFdndJREFRQUJBb0lCQVFDQWprTlh6aHVaODdiUgpVSTM0cVVZS3FhcUxaZ3c0NUEzdjluYXo1T2FRb0d6WHowK2VTejk4Q0VDamR2WXQ4RW9TOFFiL0R0R3Rob09SCmtWWXpwNnlQVU9TZlptdTBLaWo4bnk4UC9NSGdGMEQ2bmw1MEFTd1B4aHUvN3ZoRjVjQ3dnWFVzd0d3QVd1WW0KYjNqNVpJcDRZVjV6ek5ET2VXeVRrK3VmK1VIbHRxRkQ3QWU0TTl6NThyMTcvT1dodmE1bXR1c1R1dUVZanpDNgpBRS9mc09DMGdMTlNNNCtTZmNsZkNrSHBIK0dpa3pOTVNRMkgwaGxYbGxQbVI3M0JvQzZONmFvWTVoUVdCTFY3Ckx4dFliSnF4N1RBcVJ5eXBCUWVrakplMzZyb1JldFh0enkzaTZWL3k2OTA0NXRkNWtrNzBjVmptRmhsNzk0NzUKODJyblJMSEJBb0dCQU5ncTRheHI1T290VFVtUGtHZDBhZm9hV1NSUEpmaVRUZE5lTWtxVHpNNnpJY1ZMU0toQgo3OEVSd2RERDlGT3UrQmdpdmc0RGxwbUg3QXJXbjhRTkR0ZGtobVBmS1lmVHFYNnFIN0FLNGN5YnZZSUNNbGN0CkVkVzRUdkttL1pCM21yVk9QOUpWUGpkeUZNcCtKZTZOK3FwMXcrdWk5bXhYOHBXbnJDLytEZlR2QW9HQkFNWUMKR0ZqbncvTzloakYyTWIwMHFVYXJtTStyZUpaTVh2L3BWaWsrY20wZUFpWXZnR3ZLYkFZa0lYd2RiN3JMQnc5awpiYUpteFAwUHJBb1h5NVRwUGRmUk9xUHdyUkN5UmVLeW1La0VaZVRwT05nRDBzOE1iWDE2N292WnUxT1FWS1FvCkl5SmVVeldhMGtwZ2xuYkwybExWdTQ5eDhqV0hESmRZaG1rRE5FMHhBb0dBUjR1eDA3cUdNb2U1NjkzcllvSmkKVFJnSlp2NFhTRFdnN1pOZ3U5UTlWakJ0dmZvVDJ6U3ZvTXc2eE5rR2RlZ1VUeEM0ckxTOVZLVnJGNDgvbzhqYQpuNm15M1QxUVpwZEVveHExa0RPWjFubTVlRjAzd2lpMW5YSDZGMC96M3F2bmRaaW5nUHNiczRnN24yV3ZNa3lsCnFXTis2KytzOWVFSjlrUmZ0aWExQWRzQ2dZQVVuVTA1bkU5N1JjVDl5MGRjWW1vcE1GNUZhSjJ5VUJzbjIzd2IKNlNOeWxzZzBmNGVJTVZmVHY5azRtYnZ6SDRZSnBUUUF6MkE4MUcvZDBTSmh5M0tqMEdXaGdjSVMxZXlPc0hkUwpTV0h1VmhXVDc3bjMwbHhuenUrYzRic3Q5UDNLNVY3YkNpVHhsTC9GL0k1TnFlVjk4RUNKcTV4QzFGK01OaXd3CkxLUTZVUUtCZ0Myekw1OVZmOFFuUmtSTjBnT1VmczNlanJMY3hGUnpUWHZjS3FjSHRiYXF6Q3MzcVNOQzZVdlYKd2pCYXpFd1FDbzF3bk04MVg4dVQ1ZkxobmpYZWJXdG5ZZXhRbzVQMzhQaWFxVFFEZ3JiQWRoUDVQOE53UkNYTQpHM1NORXowWGVMMjdqbVdqZjBWSmR3RDBMdUhYWWhjd0FXcTRhbGhKMDI0cmpnVkh3T3plCi0tLS0tRU5EIFJTQSBQUklWQVRFIEtFWS0tLS0tCg==";
        private final String CERTIFICATE_PFX = "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";
        private final String KEYSTORE = "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";

        @Override
        public void run() {
            try {
                if (!isConnected) {
                    startSocket();
                }
                readData();
            } catch (IOException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (UnrecoverableKeyException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (CertificateException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (NoSuchAlgorithmException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (KeyStoreException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            } catch (KeyManagementException e) {
                DebugTool.logError("SSLClientReader  fail", e);
                transportListener.onTransportError("SSLClient", e);
            }
        }

        private void startSocket() throws KeyStoreException, CertificateException, NoSuchAlgorithmException, IOException, UnrecoverableKeyException, KeyManagementException {
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            InputStream is = new ByteArrayInputStream(Base64.decode(CERTIFICATE_PFX, Base64.DEFAULT));
            keyStore.load(is, "".toCharArray());
            String keyalg = KeyManagerFactory.getDefaultAlgorithm();
            KeyManagerFactory kmf = KeyManagerFactory.getInstance(keyalg);
            kmf.init(keyStore, "".toCharArray());
            SSLContext context = SSLContext.getInstance("TLSv1.2");
            context.init(kmf.getKeyManagers(),null, null);
            serverSocket = (SSLServerSocket) context.getServerSocketFactory().createServerSocket(0);
            transportListener.onTransportConnected();
            socket = serverSocket.accept();
            handshakeCompletedListener.handshakeCompleted(null);
        }

        private void readData() throws IOException {
            InputStream inputStream = socket.getInputStream();

            byte[] buffer = new byte[WiProProtocol.MTU_SIZE];
            try {
                int i;
                while ((i = inputStream.read(buffer)) != -1) {
                    transportListener.onTransportBytesReceived(buffer, i);

                    if (getRPCPacketListener() != null) {
                        getRPCPacketListener().onRPCPayloadCoded(Arrays.copyOf(buffer, i));
                    }
                }
            } catch (IOException e) {
                DebugTool.logError("sslClient read data failed ", e);
                transportListener.onTransportError("SSLClient", e);
            }
        }
    }
}
