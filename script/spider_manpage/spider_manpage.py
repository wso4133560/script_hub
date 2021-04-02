import html2text
import requests
import os


# 根据函数和manpage项目路径构建url列表
def create_urls(ltp_path):
    # 构建man 1 - 8
    url_list = []
    for pos in range(1, 9):
        sub_path = ltp_path + r'\man' + str(pos)
        url_head = "https://man7.org/linux/man-pages/man"
        for root, dirs, files in os.walk(sub_path, topdown=False):
            for name in files:
                url = url_head + str(pos) + '/' + name + '.html'
                name_url = (name[:-2], url)
                url_list.append(name_url)
    return url_list

# 根据url获取txt
def get_text(name, url):
    print(url)
    response = requests.get(url)
    response.encoding = 'utf8'
    h = html2text.HTML2Text()
    h.ignore_links = True
    #print(response.text)
    #print('*****************************************************')
    page_txt = h.handle(response.text)
    page_lines = page_txt.splitlines()
    isWrite = False
    write_fd = open('doc\\' + name + '.txt', 'w', encoding='UTF-8')
    for line in page_lines:
        if line == "* * *":
            if not isWrite:
                isWrite = True
            else:
                return

        if isWrite:
            #data = h.handle(line)
            line = line.strip('\n')
            line = line.replace("*", '')
            if line == '':
                continue
            #data = data.replace('**', '')
            write_fd.write(line)
            write_fd.write('\n')


if __name__ == "__main__":
    #get_text("https://man7.org/linux/man-pages/man3/abort.3.html", 'abort')
    url_list = create_urls(r'C:\code\open_project\man-pages')
    for name_url in url_list:
        get_text(name_url[0], name_url[1])
