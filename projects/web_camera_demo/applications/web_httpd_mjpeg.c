/*
 * Copyright (c) 2023 hpmicro
 *
 * Change Logs:
 * Date         Author          Notes
 * 2023-10-17   HPMicro     first version
 *
 */

#include <string.h>
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <sys/socket.h>
#include <lwip/opt.h>
#include <netdev.h>
#include <netdb.h>
#include "board.h"
#include "hpm_l1c_drv.h"
#include "web_httpd_mjpeg.h"
#include "cam_jpeg_encode.h"
#include "web_icon.h"

#define   NETIF_NAME         "E0"

#if defined(CONFIG_CAMERA_OV5640) && (CONFIG_CAMERA_OV5640 == 1)
#define   WEB_CAMERA_NAE     "ov5640"
#elif defined(CONFIG_CAMERA_OV7725) && (CONFIG_CAMERA_OV7725 == 1)
#define   WEB_CAMERA_NAE     "ov7725"
#else
#define   WEB_CAMERA_NAE     "NULL"
#endif

const static char http_html_hdr[] =
    "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

// __attribute__((section(".noncacheable")))
char indexdata[] = "<html> \
    <head>\
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" />\
    <body> \
    <h1 align=center style='text-align:center'>HPMicro webcam</h1> \
    <h2 align=center style='text-align:center'>RTOS:RT-Thread</h2> \
    <h2 align=center style='text-align:center'>Board:%s</h2> \
    <h2 align=center style='text-align:center'>Sensor:%s</h2> \
    <h2 align=center style='text-align:center'>Resolution:%d*%d</h2> \
    <img style='margin:0 auto;display:block;' src=\"http://%s/?%s=mjpeg\"> \
    </body> \
    </html>";

char buffer[2048]; 
rt_uint8_t video_flag = HTTP_VIDEO_STOP;
rt_uint8_t pic_num = 1;
rt_uint8_t newframe = 0;
int stream_connected;
ATTR_PLACE_AT_NONCACHEABLE rt_uint8_t jpeg_data[256 * 1024];
ATTR_PLACE_AT_NONCACHEABLE char index_data_format[2048];
static rt_sem_t mjpeg_send_sem = RT_NULL;

void http_send_icon(int client) 
{
    int frame_size = 0;
    uint16_t haed_len = 0;
    rt_sprintf(buffer,
            "HTTP/1.1 200 OK\r\n"
            "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
            "Content-type: image/x-icon\r\n\r\n"); //

    frame_size = sizeof(icon_data);
    haed_len = rt_strlen(buffer);
    rt_memcpy(&buffer[haed_len], (char *)icon_data, frame_size);
    frame_size = haed_len + frame_size; //
    send(client, buffer, frame_size, 0);
    closesocket(client);
}

http_sta http_streamer_start(int client, rt_uint8_t count) 
{
    uint16_t haed_len = 0;
    rt_sprintf(buffer,
               "HTTP/1.1 200 OK\r\n"
               "Server: HPMicro\r\n"
               "Content-Type: multipart/x-mixed-replace;boundary=%s\r\n"
               "Cache-Control: no-cache\r\n"
               "Pragma: no-cache\r\n\r\n",
               BOARD_NAME);
    haed_len = rt_strlen(buffer);
    if (send(client, buffer, haed_len, 0) == -1) {
        return http_fail;
    }
    return http_ok;
}

http_sta http_send_streamer(int client) 
{
    uint32_t frame_size = 0;
    uint16_t haed_len = 0;
    uint32_t aligned_len = 0;

    if (get_cam_jpeg_data(jpeg_data, &frame_size) == RT_FALSE) {
        return cam_timeout;
    }
    if (frame_size == 0) {
        return cam_timeout;
    }
    if (l1c_dc_is_enabled()) {
        aligned_len = (frame_size + HPM_L1C_CACHELINE_SIZE - 1U) & ~(HPM_L1C_CACHELINE_SIZE - 1U);
        l1c_dc_flush((uint32_t)jpeg_data, aligned_len);
    }
    rt_sprintf(buffer,
            "\r\n--%s\r\n"
            "Content-Type: image/jpeg\r\n"
            "Content-Length: %d\r\n\r\n",
            BOARD_NAME, frame_size);
    haed_len = rt_strlen(buffer);
    if (send(client, buffer, haed_len, 0) == -1) 
    {
        rt_kprintf("\r\nerror code:0x01\r\n");
        return http_fail;
    }
    if (send(client, jpeg_data, frame_size, 0) == -1) 
    {
        rt_kprintf("\r\nerror code:0x01\r\n");
        return http_fail;
    }
    return http_ok;
}

void http_mjpeg_send(void *parameter) 
{
    while (1) {
        if (rt_sem_take(mjpeg_send_sem, RT_WAITING_FOREVER) == RT_EOK) {
            while (1) {
                if (http_send_streamer(stream_connected) == http_fail) 
                {
                    break;
                }
                rt_thread_mdelay(30);
            }
        }
    }
}

static void handle_client(void *arg)
{
    int client_socket = (int)arg;  // 从参数获取客户端socket
    char *recv_data = rt_malloc(1024);
    char *uri;
    char http_buf[128];
    if (recv_data == NULL) {
        closesocket(client_socket);
        return;
    }
    rt_sprintf(http_buf, "GET /?%s=mjpeg HTTP/1.1", BOARD_NAME);
    while (1) {
        int bytes_received = recv(client_socket, recv_data, 1024, 0);
        if (bytes_received <= 0) {
            rt_kprintf("\r\nClient disconnected, close socket\r\n");
            break;
        }
        recv_data[bytes_received] = '\0';
        uri = strtok(recv_data, "\r\n");
        rt_kprintf("\r\nReceived URI: %s\r\n", uri);

        if (strcmp(uri, "GET / HTTP/1.1") == 0) {
            send(client_socket, http_html_hdr, rt_strlen(http_html_hdr), 0);
            send(client_socket, index_data_format, rt_strlen(index_data_format), 0);
            closesocket(client_socket);
            break;
        } else if (strcmp(uri, http_buf) == 0) {
            if (http_streamer_start(client_socket, 0) == http_ok) {
                rt_kprintf("\r\nMJPEG stream started\r\n");
                stream_connected = client_socket;
                rt_thread_mdelay(50);
                rt_sem_release(mjpeg_send_sem);
            }
        } else if (strcmp(uri, "GET /favicon.ico HTTP/1.1") == 0) {
            http_send_icon(client_socket);
            closesocket(client_socket);
            break;
        } else {
            rt_kprintf("\r\nUnknown request: %s\r\n", uri);
            closesocket(client_socket);
            break;
        }
    }
    rt_free(recv_data);
    closesocket(client_socket);
    return;
}

void http_mjpeg_server(void *parameter) 
{
    char *recv_data;
    int sock;
    struct hostent *host;
    struct sockaddr_in server_addr, client_addr;
    rt_base_t stop = 0;
    struct netdev *web_netdev = RT_NULL;
    char *ip_addr_str = RT_NULL;
    jepg_cam_init();
    recv_data = rt_malloc(1024);
    if (recv_data == NULL) {
        rt_kprintf("\r\nNo memory\r\n");
        return;
    }
    rt_kprintf("http_mjpeg_server...\r\n");
    while (1) {
        web_netdev = netdev_get_by_name(NETIF_NAME);
        if (web_netdev != RT_NULL) {
            if ((netdev_is_link_up(web_netdev) == RT_TRUE) &&
                (netdev_is_up(web_netdev) == RT_TRUE) &&
                (netdev_is_internet_up(web_netdev) == RT_TRUE)) {
                ip_addr_str = inet_ntoa(web_netdev->ip_addr);
                break;
            }
        }
        rt_thread_mdelay(1000);
    }
    rt_kprintf("web IP address has find....\n");
    host = gethostbyname(ip_addr_str);
    rt_sprintf(index_data_format, indexdata, BOARD_NAME, WEB_CAMERA_NAE, IMAGE_WIDTH, IMAGE_HEIGHT, ip_addr_str, BOARD_NAME);

    /* socketSOCK_STREAMTCPsocket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        rt_kprintf("\r\nSocket error\n");
        rt_free(recv_data);
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(WEB_SERVER_PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    rt_memset(&(server_addr.sin_zero), 8, sizeof(server_addr.sin_zero));

    /* socket */
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        rt_kprintf("Unable to bind\n");
        rt_free(recv_data);
        return;
    }

    if (listen(sock, 5) == -1) {
        rt_kprintf("Listen error\n");
        rt_free(recv_data);
        return;
    }

    rt_kprintf("WebServer Waiting for client on port %d...\n", WEB_SERVER_PORT);
    while (stop != 1) {
        uint32_t sin_size = sizeof(struct sockaddr_in);
        int client_socket = accept(sock, (struct sockaddr *)&client_addr, &sin_size);

        if (client_socket == -1) {
            rt_kprintf("\r\nAccept error\r\n");
            continue;
        }

        rt_kprintf("\nNew client connected: %s:%d\r\n", 
                 inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        rt_thread_t client_thread = rt_thread_create( "cli_th",  handle_client,  (void *)client_socket, 2048, 15, 10);

        if (client_thread != RT_NULL) {
            rt_thread_startup(client_thread);
        } else {
            rt_kprintf("Create client thread failed, close socket\r\n");
            closesocket(client_socket);
        }
    }

    closesocket(sock);
    rt_free(recv_data);
    rt_kprintf("\r\nlwip close\r\n");
    return;
}


int webcam_init(void)
{
    static uint32_t web_cam_thread_arg = 0;
    if (mjpeg_send_sem == RT_NULL)
    {
        mjpeg_send_sem = rt_sem_create("mjpeg_sem", 0, RT_IPC_FLAG_FIFO);
        if (mjpeg_send_sem == RT_NULL)
        {
            rt_kprintf("Error: Create mjpeg semaphore failed!\n");
            return RT_ERROR;
        }
    }
    rt_thread_t web_cam_server_thread = rt_thread_create("web_server_th", http_mjpeg_server, &web_cam_thread_arg, 8192, 5, 10);
    if (web_cam_server_thread != RT_NULL) {
        rt_thread_startup(web_cam_server_thread);
    } else {
        rt_kprintf("Create web_cam server thread failed!\n");
        return RT_ERROR;
    }
    rt_thread_t mjpeg_send_thread  = rt_thread_create("mjpeg_send", http_mjpeg_send, &web_cam_thread_arg, 8192, 5, 10);
    if (mjpeg_send_thread  != RT_NULL) {
        rt_thread_startup(mjpeg_send_thread);
    } else {
        rt_kprintf("Create mjpeg send thread failed!\n");
        return RT_ERROR;
    }
    return RT_EOK;
}
