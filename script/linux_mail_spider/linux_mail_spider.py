import urllib3.request
from lxml import etree


headers = {
     'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36'
}


def write_url(base_path, curr_page, write_fd):
    url = base_path + curr_page
    print(url)
    proxy = urllib3.ProxyManager('http://127.0.0.1:10809', headers=headers)
    response = proxy.request('get', url)
    html = etree.HTML(response.data.decode())
    # 获取href
    href_list = html.xpath('//a/@href')
    # 获取content
    subject_list = html.xpath('//a/text()')
    next_page = ''
    for pos in range(0, len(href_list)):
        if 'Next Page' == subject_list[pos]:
            next_page = href_list[pos]
            continue

        if 'Thread Index' == subject_list[pos]:
            continue

        if href_list[pos][-5:] != '.html':
            break

        # 提取标头
        write_fd.write(subject_list[pos] + '\n')
        # 构建url
        write_fd.write(base_path + href_list[pos] + '\n\n')
    return next_page


# 根据url获取链接头
def get_mail_subject(base_path, filename):

    write_fd = open(filename + '.txt', 'w',  encoding='UTF-8')
    next_page = 'maillist.html'
    while next_page != '':
        next_page = write_url(base_path, next_page, write_fd)
    write_fd.close()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    #get_mail_subject('https://www.spinics.net/lists/kvm/', 'kvm')
    get_mail_subject('https://www.spinics.net/lists/linux-api/', 'linux-api')
