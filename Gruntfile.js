module.exports = function (grunt) {
	grunt.initConfig({
		exec: {
			clean: {
				cmd: function () {
					return "git clean -x -f";
				}
			},
			preprocess: {
				cwd: "./articles",
				cmd: function () {
					var command = "review-preproc";
					var files = [
						"01-sample"
					];
					var exec = command + " -r --tabwidth=2 " + files.map(function(file){ return file + ".re"; }).join(" ");
					console.log(exec);
					return exec;
				}
			},
			compile2text: {
				cwd: "./articles",
				cmd: function () {
					return "review-compile --all --target=text --subdirmode";
				}
			},
			compile2html: {
				cwd: "./articles",
				cmd: function () {
					return "review-compile --all --target=html --subdirmode";
				}
			},
			compile2latex: {
				cwd: "./articles",
				cmd: function () {
					return "review-compile --all --target=latex --subdirmode";
				}
			},
			compile2idgxml: {
				cwd: "./articles",
				cmd: function () {
					return "review-compile --all --target=idgxml --subdirmode";
				}
			},
			compile2pdf: {
				cwd: "./articles",
				cmd: function () {
					return "review-pdfmaker config.yml";
				}
			},
			compile2epub: {
				cwd: "./articles",
				cmd: function () {
					return "review-epubmaker config.yml";
				}
			},
			readme: {
				cmd: function () {
					return "review-compile --target markdown README.re > README.md";
				}
			}
		}
	});

	function generateTask(target) {
		return ['clean', 'exec:preprocess', 'exec:compile2' + target];
	}

	grunt.registerTask(
		'default',
		"原稿をコンパイルしてPDFファイルにする",
		"pdf");

	grunt.registerTask(
		'clean',
		"git clean -x -f を使っていらないファイルを全部消す",
		"exec:clean");

	grunt.registerTask(
		'text',
		"原稿をコンパイルしてTextファイルにする",
		generateTask("text"));

	grunt.registerTask(
		'html',
		"原稿をコンパイルしてHTMLファイルにする",
		generateTask("html"));

	grunt.registerTask(
		'idgxml',
		"原稿をコンパイルしてInDesign用XMLファイルにする",
		generateTask("idgxml"));

	grunt.registerTask(
		'pdf',
		"原稿をコンパイルしてpdfファイルにする",
		generateTask("pdf"));

	grunt.registerTask(
		'epub',
		"原稿をコンパイルしてepubファイルにする",
		generateTask("epub"));

	grunt.registerTask(
		'readme',
		"README.reからREADME.mdを生成する(GitHub用)",
		["exec:readme"]);

	require('matchdep').filterDev('grunt-*').forEach(grunt.loadNpmTasks);
};
