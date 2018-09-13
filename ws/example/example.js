//与服务器的连接和数据传递


// id=sex对应生成性别的饼图
function showPie() {
	var myChart = echarts.init(document.getElementById('sex'));
	var option = {
		title: {
			text: '员工性别分布情况',
			left:'center',
			top: 'bottom',
			textStyle: {
				fontSize: 24
			}
		},
		series: [{
			//roseType: 'angle',
			type: 'pie',
			radius: '70%',
			data: [
				// value根据数据库数值输入
				{value:20, name:'男', itemStyle: {color: '#8FBC8F'}},
				{value:5, name: '女', itemStyle: {color: '#FF8C00'}}
			],
			label: {
				textStyle: {
					fontSize: 18
				}
			}
		}]
	};
	myChart.setOption(option);
}
// id=spl对应生成特长的柱状图
function showBar() {
	var myChart = echarts.init(document.getElementById('spl'));
	var option = {
		title: {
			text:'员工特长分布情况',
			left:'center',
			top:'bottom',
			textStyle: {
				fontSize: 24
			}
		},
		tooltip: {},
		legend: {
			data:['人次'],
			x: 'left',
			y: 'top',
			textStyle: {
				fontSize: 18
			}
		},
		xAxis: {
			data:['游泳','跑步','篮球','足球'],
			axisLabel: {
				textStyle: {
					fontSize: 18
				}
			}
		},
		yAxis: {
			axisLabel: {
				textStyle: {
					fontSize: 18
				}
			}
		},
		series: [{
			color: '#87CEEB',
			barWidth: '50px',
			name: '人次',
			type:'bar',
			// 此项数据根据数据库传入！
			data: [10,3,6,20]
		}]
	};
	myChart.setOption(option);
}