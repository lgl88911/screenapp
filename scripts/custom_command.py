from west.commands import WestCommand
import subprocess

class CustomFlashCommand(WestCommand):
    def __init__(self):
        super().__init__(
            'custom-flash',
            'Add header and download firmware',
            '')

    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(self.name, help=self.help,
                                         description=self.description)
        return parser

    def do_run(self, args, unknown_args):
        # 执行第一条命令
        subprocess.run(['./tools/mm-sdk/exe/mm', 'add_header', '-f', 'build/zephyr/zephyr.bin', '-a', '0x80000000'])
        
        # 执行第二条命令
        subprocess.run(['./tools/mm-sdk/exe/mm', 'download', '-t', 'partition', '-f', 'zephyr.bin.img'])

        print("Custom flash command completed successfully.")