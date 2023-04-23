<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?php echo $title;?></title>
</head>
<style>
    article {
        border: 3px solid rgb(195, 2, 2);
        width: 800px;
        margin: 0 auto;
        padding-bottom: 50px;
    }

    p {
        display: flex;
        justify-content: space-between;
        border-bottom: 2px dotted rgb(79, 205, 233);
        margin: 0;
    }

    p:nth-child(1) {
        border-top: 2px dotted rgb(79, 205, 233);
    }

    p:nth-child(2) {
        padding-bottom: 40px;
        border-bottom: 0;
    }

    input[type="button"] {
        margin-left: 10px;
    }
</style>

<body>
<article>
    <section></section>
    <!-- 增加一个多行输入文本框 -->
    <textarea name="" id="" cols="30" rows="10"></textarea>
    <input type="button" value="留言">
</article>
<script type="text/javascript">
    //获取节点
    let artEle = document.querySelector("article");
    let secEle = document.getElementsByTagName("section")[0];

    //创建数组
    let arr = [
        { name: "小张", time: "2020-01-01", content: "今天星期五，明天星期六！" },
        { name: "小磊", time: "2020-02-01", content: "中秋节快乐！" },
        { name: "小海", time: "2020-03-01", content: "明天不上课！爽翻！！！" },
        { name: "小丽", time: "2020-04-01", content: "今晚做个好梦哦~" },
    ]

    //将数据渲染到页面上
    function render() {
        let str = "";
        arr.forEach((item, index) => {
            str += `
        <div>
           <p>
              <span>${item.name}</span>
              <span>${item.time}</span>
           </p>
           <p>
              <span>${item.content}</span>
              <span data-index="${index}">删除</span>
           </p>
        </div>
        `;
        });
        secEle.innerHTML = str;
    }

    //给祖先元素article绑定一个事件监听器（事件代理）
    artEle.addEventListener("click", function (e) {
        let index = e.target.dataset.index - 0;
        if (e.target.value == "留言") {
            let textAreaEle = document.getElementsByTagName("textarea")[0];

            //获取本地时间
            let date = new Date();
            let localTime = date.toLocaleString();//年月日、时分秒同时获取
            //将原本的/ 换成-
            //replace该方法主要用于将字符串中符合匹配条件的字串替换成其他的字符串，
            //返回替换后的字符串，且原字符串不变。
            let gang = localTime.replace(/\//g, "-");
            //将用户输入的内容容作为对象存进数组
            arr.push(
                { name: "小明", time: gang, content: textAreaEle.value }
            )
            //清空输入框，方便下次输入
            textAreaEle.value = "";
            //再次渲染
            render();
        }
        if (e.target.innerHTML == "删除") {
            arr.splice(index, 1);
            render();
        }
    })
    render();

</script>
</body>

</html>
