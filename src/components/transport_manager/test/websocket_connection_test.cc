/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "gtest/gtest.h"
#include "resumption/last_state_impl.h"
#include "transport_manager/cloud/cloud_websocket_transport_adapter.h"
#include "transport_manager/cloud/sample_websocket_server.h"
#include "transport_manager/cloud/websocket_client_connection.h"
#include "transport_manager/transport_adapter/connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "transport_manager/mock_transport_manager_settings.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using namespace ::transport_manager;
using namespace ::transport_manager::transport_adapter;
namespace websocket = sample::websocket;

class WebsocketConnectionTest : public ::testing::Test {
 public:
  struct WebsocketClient {
    std::shared_ptr<CloudWebsocketTransportAdapter> adapter;
    std::shared_ptr<WebsocketClientConnection> connection;
  };

  void InitWebsocketClient(
      const transport_manager::transport_adapter::CloudAppProperties&
          properties,
      WebsocketClient& client_out) {
    uniq_id = dev_id = properties.endpoint;

    client_out =
        WebsocketClient{std::make_shared<CloudWebsocketTransportAdapter>(
                            last_state_, transport_manager_settings),
                        nullptr};
    client_out.adapter->SetAppCloudTransportConfig(uniq_id, properties);

    TransportAdapterImpl* ta_cloud =
        dynamic_cast<TransportAdapterImpl*>(client_out.adapter.get());
    ta_cloud->CreateDevice(uniq_id);

    auto connection = client_out.adapter->FindPendingConnection(dev_id, 0);

    ASSERT_NE(connection, nullptr);

    client_out.connection =
        std::dynamic_pointer_cast<WebsocketClientConnection>(connection);

    ASSERT_NE(client_out.connection.use_count(), 0);
  }

  void StartWSServer(std::string path) {
    ws_session = std::make_shared<websocket::WSSession>(kHost, kPort);
    ws_session->AddRoute(path);
    ws_session->Run();
  }

  void StartWSSServer(std::string path) {
    wss_session = std::make_shared<websocket::WSSSession>(
        kHost, kPort, kCertificate, kPrivateKey);
    wss_session->AddRoute(path);
    wss_session->Run();
  }

 protected:
  WebsocketConnectionTest()
      : last_state_("app_storage_folder", "app_info_storage") {}

  ~WebsocketConnectionTest() {}

  void SetUp() OVERRIDE {
    ON_CALL(transport_manager_settings, use_last_state())
        .WillByDefault(Return(true));
  }

  NiceMock<MockTransportManagerSettings> transport_manager_settings;
  resumption::LastStateImpl last_state_;
  std::string dev_id;
  std::string uniq_id;
  std::shared_ptr<websocket::WSSession> ws_session;
  std::shared_ptr<websocket::WSSSession> wss_session;
  WebsocketClient ws_client;
  std::string kHost = "127.0.0.1";
  uint16_t kPort = 8080;
  std::string kPath = "/folder/file.html/";
  std::string kQuery = "?eventId=2345&eventName='Test'&expectedResult=true";
  std::string kFragment = "#section_1";

  // Sample certificate for localhost
  std::string kCertificate =
      "-----BEGIN "
      "CERTIFICATE-----\nMIIDqTCCApECCQC/"
      "5LlQ+"
      "GLgqTANBgkqhkiG9w0BAQsFADAXMRUwEwYDVQQDDAwx\nMjcuMC4wLjEgQ0EwIBcNMTkwNTA"
      "2MTkzMjM2WhgPMjExOTA0MTIxOTMyMzZaMBQx\nEjAQBgNVBAMMCTEyNy4wLjAuMTCCAiIwD"
      "QYJKoZIhvcNAQEBBQADggIPADCCAgoC\nggIBALbeIoAHFaFeNqbMnhtAO5QTgRd0X9qxpB0"
      "d4M0dEeog2l/+inNA/eu+CCvm\nJj2I8+6MWH2TUrl/"
      "2xfhHjzsMrtISCdpNcjaNzCnvi8ZcspFi3evknvs3+uo2/"
      "wn\nyNZ04jp0oQ0k1cZ6WBpLTYV7WgmueemiWEiAfw+YE+wtRg+"
      "0H7rksrbpeNPnxQHX\nBDDkWqwvfkD15Sd0XFQkW27K72/"
      "et2uKuAcJHCIUbsA4iZyJw4Uu4eusy7W5kddX\nThE7Y1WqTXyA4j/"
      "ZCYigXmsHbWrikPLVXbORhiMF4/60u8RDs0jI8xtgRQhLR9Vp\n3xys7/"
      "5tHZX00s6x6OVy8aMSZIIVS0eWoVN8bGd9B4+fDMOcNT0YODeQA10w/"
      "85P\nEiZDQ8AxneQkh8H3qjD2+"
      "G9oHZaTk0zHTyaKRg3xGP3N9C96onaJX4Rm6nNvApO8\nU7lQ+xHkLwjKCQke39W+"
      "r3FHwwQvUDeoJBXf6iVkIMFoUPAHNZqKf9Db6KKIEp8i\nDgIhbBxiB4MSn36Zly4SOMojyM"
      "ZFri+"
      "HzMbuHFlm8e8QRWGmM4UM2rHIpl4OJolg\nejesDqO8YZR5mZXV0FJRiPgLo2Q4OTtb3tEHJ"
      "ZLmlT+"
      "f42bge4ZCZmGPrkNfr68Y\nDEnJ6z4ksOVkMefOp2SNYLYYGPYyiKwa9qVkH9Obkect1omNA"
      "gMBAAEwDQYJKoZI\nhvcNAQELBQADggEBABFJQtOTftrzbLBA5Vm6aPkbUyxhcaOpz+d+"
      "Ljd6pIs4H+"
      "Eb\nXkoHhmay4stZkDc2HVSKESZloI3Ylup8z3aRJjfOexJqHlYzk2vraYryWm8odgID\n5V"
      "B0Zle8ofpHTJw1LCOXHkzKt1G6vdQpuq/"
      "4OKpmggaIqqEC1bfOYZt5t6vIx3OF\nxjPz91NTR9gZ4lKrho1+sfS+"
      "jbSjKkqVAhE4yTpKLPHRshQnBFEhvATXNvdZGftF\n+tXxqKsBZ9aX0/"
      "YmPFIXFcjdjSSiuq1F1DjiqIZo88qfa9jlTg6VJdayyQ/"
      "cu41C\nBucY8YTF0Ui8ccIS55h2UTzPy5/4PbrwI4P+Zgo=\n-----END "
      "CERTIFICATE-----\n";
  // Sample private key
  std::string kPrivateKey =
      "-----BEGIN RSA PRIVATE "
      "KEY-----\nMIIJKAIBAAKCAgEAtt4igAcVoV42psyeG0A7lBOBF3Rf2rGkHR3gzR0R6iDaX/"
      "6K\nc0D9674IK+YmPYjz7oxYfZNSuX/"
      "bF+EePOwyu0hIJ2k1yNo3MKe+LxlyykWLd6+S\ne+zf66jb/"
      "CfI1nTiOnShDSTVxnpYGktNhXtaCa556aJYSIB/"
      "D5gT7C1GD7QfuuSy\ntul40+fFAdcEMORarC9+"
      "QPXlJ3RcVCRbbsrvb963a4q4BwkcIhRuwDiJnInDhS7h\n66zLtbmR11dOETtjVapNfIDiP9"
      "kJiKBeawdtauKQ8tVds5GGIwXj/rS7xEOzSMjz\nG2BFCEtH1WnfHKzv/"
      "m0dlfTSzrHo5XLxoxJkghVLR5ahU3xsZ30Hj58Mw5w1PRg4\nN5ADXTD/"
      "zk8SJkNDwDGd5CSHwfeqMPb4b2gdlpOTTMdPJopGDfEY/"
      "c30L3qidolf\nhGbqc28Ck7xTuVD7EeQvCMoJCR7f1b6vcUfDBC9QN6gkFd/"
      "qJWQgwWhQ8Ac1mop/"
      "\n0NvooogSnyIOAiFsHGIHgxKffpmXLhI4yiPIxkWuL4fMxu4cWWbx7xBFYaYzhQza\nscim"
      "Xg4miWB6N6wOo7xhlHmZldXQUlGI+AujZDg5O1ve0QclkuaVP5/"
      "jZuB7hkJm\nYY+uQ1+"
      "vrxgMScnrPiSw5WQx586nZI1gthgY9jKIrBr2pWQf05uR5y3WiY0CAwEA\nAQKCAgBGSDGyS"
      "wbBMliG2vWZO6KqUqS2wv9kKgoNNsKDkryj42SKqGXFziDJTgwN\n8zKXS9+Uu1P3T3vn13/"
      "5OYhJme4VlL5Gh2UogNXdWVr69yjrHLdxlIUUJAIbrJZ/"
      "\n3zqNUfbwyIptZs7SrYrW8EInHzWHqwsoBEEx/"
      "FDZSXW+u9fFiVD4n5UgP7M0nktV\nXbI6qElBDC/V/"
      "6vG8i3aGO8bMdu8fzi3mGUKLzIk1v2J2zDofPosYcxqq8rPWTb4\nMJHMhaqz7fRB+"
      "bb7GwtS+2/Oathe0B0td1u//Bo1s7ng1s2jrPFm8/"
      "SbfPCLM4O0\nPjCF8OF8Q6uvSp0K283LAdZk+liuDztc/"
      "Ed8mcnCZQhBp86mJQi0Jj3Mje7prOAY\nXojMroYj7r2igCJvcwGb1y6zZWSj3jXuHze3bLy"
      "fv7pCD+hkiZR7mZhQpOhQRZaU\ntdFGc+"
      "DuykxKPqVjAPy7iVQXYnMjpo36SGIWoZLuepQto3GvU6czyOwhK0/"
      "Mwbwd\nZpYpLH3L9IetY8GcPefmUR5wQUlUTrpxgGElIzkkWW8zmUWBXwrGbAtN1HJWpJdN"
      "\neVshKod2fo03IQMPywSdENCJVeSrgRMuwPyFaOM+"
      "CVrBJwD66K9YWn4cVRUIZsTq\nAXhQ8DzF+WCOZshhMUbCJX+KpcOFI8nxOrPp+"
      "J1s1YpLLvdmcQKCAQEA7bwvOiCD\nHvaqpYg1jJak6l/"
      "iY3QIOOpFyjfYrQXS0BNRmmxK8Lzevi/"
      "NPTqu146QKDaIGvzu\n+"
      "bXnuV1LmZqnOm5J6Kdx0Mk4Fb88akRtS9gOdzU7WWMYIGkeF1oih0ZrhHuIey6e\nCeLSmJh"
      "UDaTIWcCirALRHcLWUS6BTGGuE+up5QG7djIW/"
      "LD17lhGE6fXPlbYXb7l\nPbYwL1Yr0urIp9Un+zrExw+77TTGK7T37T1ZJv46qnro0/"
      "HK8XxZ+"
      "JNc18F763O4\nPmwu8KWrx4qnyPYAuB1rbVntK6UxYks9jvk93XecCUP6HHp0I0tV/"
      "kBNd2+"
      "18zux\n033xFEVKrUksmwKCAQEAxOrPGyBVFp29V4gQsMQdmafkDAgCLREgjQTOamFsYL5+"
      "\nZWq+6VhT/"
      "B650sIoRdX0N8timnSLBfjeqaBG5VVhhyKZhP5hulfRjALhdbGBOYvf\n0gLSffImwWdYQfx"
      "jtCSO+"
      "XCLVdAJGOOWeBVVKzH18ZvCFQwUr3Yrp7KmKWKAqgf7\n6rYovWh8T5LLYS0NzXCEwf9nJ0N"
      "JMOy7I9B7EtF8Cs6tK3aaHVqDz7zufwosS7gI\n3aI51Qh4a5D1p95+"
      "YU09beWjWGYnPiCKk4D47zaeOe7OQINaWugExlelHtJh9unO\nnhOFXhziav2Kxq1CICAuXl"
      "Vx3A+gn/cU3niNHz2A9wKCAQEAws+aw78ws4bef5cG\nipZHveek1GqY8krHtdXdsKs8/"
      "VVXYXusTWn3/VGelbYo4GrqpolJLxRloCr4IGXb\nNZwNvUvzNLtCAR1i4C89irdX+Paro/"
      "PzFmSluKlrByfNc5y5Lm8sgATLbL56ZKEu\n/58wrpu0sc/"
      "9HK40gYHiYn0I8ToElqy8uTaCr78zSIT9p826DFOOKgPsRo2tHp02\nfDf5Bc8eXDjkV1sFX"
      "HQKkHZTVA0ZqWJbIKhncoaJDyofcBsR0ZuzuFWzfTOZo4mf\nInz00TEFldpF1e4C8+"
      "kCdtHBOA/2Ki2Bp/YUVpHh6aoqZZa75Euehhs8tVpW242M\njEOSUQKCAQAM64sjMH/kt/"
      "zQXXEa6AM5LbbcwznBUzpbhlE00aeWwWjxpotYLB92\nj12J4otZ6avYbVPO5o6omaeiYY3F"
      "RlDb2P1RqI8o9tIc6aN5YWglKnRJBz5gXR8F\n2Y4E5lZ0X2GyJBxASSIPq/"
      "8Xae7ooqKMc7fMQbqpuIssuaAFXx0qCtQQllsd8lkV\nr4AApEAflp5fTC6seNG4kA/"
      "HTcqFdZE59E2QaHu8KVA0tSTA2R4G6dBLGnXI8IFW\nLXCwzvxjzfmV2FdbWXiBrwjonLG4o"
      "FDJZE3MFdI73LVTfjSrTQp4dObFoGpDvols\nk64jUwLfsLzaG6kY0z2qwT9xSV+"
      "ZCSQJAoIBADsSBeyELc5KnmOhT0Xue2o0bkAS\n8KcGWdAoQlaLCIs3vtdlC7DXDXO1l8FkT"
      "Qrd+GwP3eWDq6ymtO4vkJ3m4cZ1PEsv\n6Qet7qtkVhUfa+"
      "FYKtDzSMRuSaFPW3Ro1PesFa1H3y+sw5HL36vhRaSK+T4Wy5k2\nf7EwNZIf/"
      "ZZeA+"
      "sEtqj33obOBIiuU9htAjN81Kz4a0Xlh6jc4q6iwYKL8nZ76JYV\n8hXWIz6OXxLXE158+"
      "QtJSZaRCvdr6L5UtWfMPKSMqgfhXfTYViPDleQCkJ132mIS\nH28UoET0Y5wI8M6pMkWpSqW"
      "WcKPFGwyLInvHdxgnTAsutowkldA7qFwoRz4=\n-----END RSA PRIVATE KEY-----\n";
  // Sample CA certificate(used to sign the server certificate)
  std::string kCACertificate =
      "-----BEGIN "
      "CERTIFICATE-----"
      "\nMIIDBjCCAe6gAwIBAgIJAPyCrKRDl3SWMA0GCSqGSIb3DQEBCwUAMBcxFTATBgNV\nBAMM"
      "DDEyNy4wLjAuMSBDQTAgFw0xOTA1MDYxOTMyMzZaGA8yMTE5MDQxMjE5MzIz\nNlowFzEVMB"
      "MGA1UEAwwMMTI3LjAuMC4xIENBMIIBIjANBgkqhkiG9w0BAQEFAAOC\nAQ8AMIIBCgKCAQEA"
      "xrNHoY5+"
      "JCgnyvEdG2dvvOaZ3sg6uhuF5Ssb5snyo9ixrxO5\nZkDGWwUqFO9PiYuO5ovNeq9LZtPG6s"
      "K8zsCS062ChZX/7tZHndf4MKCUDzv/"
      "LPHe\nnROoPi9n2FAiOPctY5hpgpIJDPI5Ofx0el2KPeFGeUO/"
      "W3yqnfol1ZqzZ2h3uErR\noHkgT2Ja4K+5gnPkr/"
      "RluJIu3AmWYw4eKi8i3+"
      "PoThGmCFvoGfcWvRoctgnOHYHE\n4bDRirXL9nGkZ5FMzOVDeoIAEAShOqJwL08VcY+Pg/"
      "qFQjzRrTmiKgm6QsHNnm0q\nzg70XaD88VJimiGYZOuJHNZpX8o0W+1Ls/"
      "NbawIDAQABo1MwUTAdBgNVHQ4EFgQU\nkW3hgFWYMpVUkq9AlzGlI3awTGEwHwYDVR0jBBgw"
      "FoAUkW3hgFWYMpVUkq9AlzGl\nI3awTGEwDwYDVR0TAQH/BAUwAwEB/"
      "zANBgkqhkiG9w0BAQsFAAOCAQEAZeMkILiG\neEcELWb8cktP3LzvS47O8hys9+"
      "6TFmftuO7kjDBd9YH2v8iQ7qCcUvMJ7bqL5RP1\nQssKfNOHZtw/"
      "MMcKE6E3nl4yQSKc8ymivvYSVu5SYtQCedcgfRLb5zvVxXw8JmCp\nGNS3/"
      "OlIYZAamh76GxkKSaV3tv0XZB6n3rUVQZShncFbMpyJRW0XWxReGWrhXv4s\nNxMeC1r07EE"
      "WIDecv8KKf1F8uT4UF48HnC0VBpXiOyDGvn35NiKp+"
      "Q5k7QV6jdCS\ngPRcnZhs6jiU0jnV8C9A1A+"
      "3pXSSPrAed7tvECOgHCfS10CLsLWsLuSjc93BE5Vt\nav7kmxSwrdvQ2A==\n-----END "
      "CERTIFICATE-----\n";
  std::string kIncorrectCertificate =
      "-----BEGIN "
      "CERTIFICATE-----\nMIIC/"
      "jCCAeagAwIBAgIJAIZjLucUID1mMA0GCSqGSIb3DQEBCwUAMBQxEjAQBgNV\nBAMMCTEyNy4"
      "wLjAuMTAeFw0xOTA1MDYxNDA1MzdaFw0yOjA1MDZxNDA1MzdaMBQx\nEjAQBgNVBAMMCTEyN"
      "y4wLjAuMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\nggEBALfE5Qhc2mHTIux30l7"
      "eHFjFLdzYeXE8vcaXKodalCG8EkRxojDOfUv+y2DV\nJzHAMiGxMFAEcSz71k+"
      "jFZwhNG8PLY0b36ChnUsrGkOSJWq3OKUFrg8KxO9At9dL\nJsa+"
      "R0N0D1bMoPYdpCi3m0b0q2ITHe56qKuTLTrIPia+"
      "qXGEVD7EoEhU9tnwlcwE\npsUURMXCn2+FhHyqN9wjFkldmu4k8U3OJOK4385L+"
      "4RJoIV8dsYawAMAf+"
      "WuxyWq\niPQTPxr8q33ZZm6z0XrgsgxHYCCsryx8L9Ub9Zwu0mie7eL63rYdr86gULvnF1bY"
      "\ncOunKFc71rBYFalbD6YYsre733kCAwEAAaNTMFEwHQYDVR0OBBYEFKW9ByUNG84Z\nYiSc"
      "hUrB7KV9FinZMB8GA1UdIwQYMBaAFKW9ByUNG84ZYiSchUrB7KV9FinZMA8G\nA1UdEwEB/"
      "wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAHYROS1EL2qnrwmAsR6c\nqo8yrk8qzLKt4os"
      "41nv7QMUQFIO+QLPE8SbH4fK1YOMMUJ4ppARQBDaIIR3UEhp1\nZHT/"
      "wGjK9qxcuQ1EXLyHOY0rxS5q57dYGxOyQo4v6IoLVMZ1ij+RJGPYI/"
      "2fDXs0\nbDldeJyC1voHmG6lqTN5nLG7Y3j9j6rtSqJyRz5muaecQNiFPQOM2OTp0rC4VeAF"
      "\ndejmTmLhUFVuHMzLF+"
      "bpzsN76GnnQquJy2jexzFoWgbEfFVLKuyhTHQAalRb4ccq\nXCIx1hecNDYRY3Sc2Gzv5qxk"
      "kWF8zqltT/0d5tx0JwN3k5nP4SlaEldFvD6BELxy\nVkU=\n-----END "
      "CERTIFICATE-----\n";
};

TEST_F(WebsocketConnectionTest, WSConnection_SUCCESS) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = "ws://" + kHost + ":" + std::to_string(kPort),
      .certificate = "no cert",
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSServer, this, "/");
  usleep(5000);

  // Start client
  InitWebsocketClient(properties, ws_client);
  std::shared_ptr<WebsocketClientConnection> ws_connection =
      ws_client.connection;

  // Check websocket connection
  TransportAdapter::Error ret_code = ws_connection->Start();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop client
  ret_code = ws_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  ws_session->Stop();
  t1.join();
}

TEST_F(WebsocketConnectionTest, WSConnection_SUCCESS_ValidTarget) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = "ws://" + kHost + ":" + std::to_string(kPort) + kPath +
                  kQuery + kFragment,
      .certificate = "no cert",
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSServer, this, kPath);
  usleep(5000);

  // Start client
  InitWebsocketClient(properties, ws_client);
  std::shared_ptr<WebsocketClientConnection> ws_connection =
      ws_client.connection;

  // Check websocket connection
  TransportAdapter::Error ret_code = ws_connection->Start();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop client
  ret_code = ws_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  ws_session->Stop();
  t1.join();
}

TEST_F(WebsocketConnectionTest, WSConnection_FAILURE_InvalidTarget) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = "ws://" + kHost + ":" + std::to_string(kPort) + kPath +
                  kQuery + kFragment,
      .certificate = "no cert",
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSServer, this, "/");
  usleep(5000);

  // Start client
  InitWebsocketClient(properties, ws_client);
  std::shared_ptr<WebsocketClientConnection> ws_connection =
      ws_client.connection;

  // Check websocket connection
  TransportAdapter::Error ret_code = ws_connection->Start();
  EXPECT_EQ(TransportAdapter::FAIL, ret_code);

  // Stop client
  ret_code = ws_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  ws_session->Stop();
  t1.join();
}

TEST_F(WebsocketConnectionTest, WSSConnection_SUCCESS) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = "wss://" + kHost + ":" + std::to_string(kPort),
      .certificate = kCACertificate,
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WSS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSSServer, this, "/");
  usleep(5000);

  // Start client
  InitWebsocketClient(properties, ws_client);
  std::shared_ptr<WebsocketClientConnection> wss_connection =
      ws_client.connection;

  // Check websocket connection
  TransportAdapter::Error ret_code = wss_connection->Start();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop client
  ret_code = wss_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  wss_session->Stop();
  t1.join();
}

TEST_F(WebsocketConnectionTest, WSSConnection_SUCCESS_ValidTarget) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = "wss://" + kHost + ":" + std::to_string(kPort) + kPath,
      .certificate = kCACertificate,
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WSS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSSServer,
                 this,
                 (kPath + kQuery + kFragment));
  usleep(5000);

  // Start client
  InitWebsocketClient(properties, ws_client);
  std::shared_ptr<WebsocketClientConnection> wss_connection =
      ws_client.connection;

  // Check websocket connection
  TransportAdapter::Error ret_code = wss_connection->Start();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop client
  ret_code = wss_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  wss_session->Stop();
  t1.join();
}

#ifdef ENABLE_SECURITY
TEST_F(WebsocketConnectionTest, WSSConnection_FAILURE_InvalidTarget) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = "wss://" + kHost + ":" + std::to_string(kPort),
      .certificate = kCACertificate,
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WSS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSSServer, this, kPath);
  usleep(5000);

  // Start client
  InitWebsocketClient(properties, ws_client);
  std::shared_ptr<WebsocketClientConnection> wss_connection =
      ws_client.connection;

  // Check websocket connection
  TransportAdapter::Error ret_code = wss_connection->Start();
  EXPECT_EQ(TransportAdapter::FAIL, ret_code);

  // Stop client
  ret_code = wss_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  wss_session->Stop();
  t1.join();
}

TEST_F(WebsocketConnectionTest, WSSConnection_FAILURE_IncorrectCert) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = "wss://" + kHost + ":" + std::to_string(kPort),
      .certificate = kIncorrectCertificate,
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WSS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSSServer, this, "/");
  usleep(5000);

  // Start client
  InitWebsocketClient(properties, ws_client);
  std::shared_ptr<WebsocketClientConnection> wss_connection =
      ws_client.connection;

  // Check websocket connection
  TransportAdapter::Error ret_code = wss_connection->Start();
  EXPECT_EQ(TransportAdapter::FAIL, ret_code);

  // Stop client
  ret_code = wss_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  wss_session->Stop();
  t1.join();
}
#endif  // ENABLE_SECURITY
}  // namespace transport_manager_test
}  // namespace components
}  // namespace test
