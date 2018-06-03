# code summary ui dialog to allow user select folder to execute code summary.
# files are currently not supported, since QFileDialog is not easily to allow
# both file folder and files selected both for one dialog.
import sys
import codesummary
from PyQt5 import QtCore, QtGui, QtWidgets

class CodeSumDialog(QtWidgets.QDialog):
    def __init__(self):
        super().__init__()

        self.setObjectName("CodeSumDialog")
        self.resize(395, 97)
        self.setWindowTitle("Code Summary")
        self.widget = QtWidgets.QWidget(self)
        self.widget.setGeometry(QtCore.QRect(60, 12, 278, 61))
        self.widget.setObjectName("widget")

        self.verticalLayout = QtWidgets.QVBoxLayout(self.widget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")

        self.label = QtWidgets.QLabel(self.widget)
        self.label.setObjectName("label")
        self.label.setText("Specify folder to execute code summary")

        self.pushButton = QtWidgets.QPushButton(self.widget)
        self.pushButton.setObjectName("pushButton")
        self.pushButton.setText("file browser")

        self.verticalLayout.addWidget(self.label)
        self.verticalLayout.addWidget(self.pushButton)

        # self.pushButton.clicked.connect(self.invokeFileBrowser())
        # note this is wrong connect to invokeFileBrowser() which return nothing.
        # we are bing to function object not its return value.
        self.pushButton.clicked.connect(self.execCodeSumForFolder)

    def execCodeSumForFolder(self):
        # browser = QtWidgets.QFileDialog() will not show this file dialog
        # we need to pass its parent. so its lifetime can extend not limit to this func.
        # browser = QtWidgets.QFileDialog(self)
        # or make browser non-local, as member of this class.
        # self.browser = QtWidgets.QFileDialog(self)

        # allow both for dir and file.
        #self.browser.setFileMode(2)

        # in window, default is using native file dialog for choosing folder.
        # the native file dialog for folder does not show files but only dirs.
        self.fileSpec = QtWidgets.QFileDialog.getExistingDirectory\
            (self, 'open folder', 'D:\\', QtWidgets.QFileDialog.DontUseNativeDialog)

        codesummary.CalCodeLinesForDir(self.fileSpec)

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    mainDialog = CodeSumDialog()
    mainDialog.show()
    sys.exit(app.exec_())