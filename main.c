#include "led.h"
#include "encoder.h"
#include "interrupt.h"
#include "uart.h"
#include "system.h"
#include "display.h"
#include <stm32f1xx.h>
#include <stdio.h>
#include <inttypes.h>

unsigned char letters[] = "������-������ � ��Ϥ�� ���Ԧ "
			  "��� ��ϧ �����, ��� ��ϧ �������. "
			  "������, ������� æ ��� ������Φ �����: "
			  "\"� ���� ��� �'���, � ���� ��� ���������\". "
			  "������ ��Φ, ���� � ���Ӧ� ���� "
			  "� ����Φ �����, � �������� �Ԧ���. "
			  "������, ���� ������� �Ϥ ���� � �����, "
			  "�� ��Ϥ � ���� ������ �� ��̦���.\n"
			  "� ����� ����, ����, ���, ���, ������, �'��� - "
			  "�����Ԧ� æ��� �������. "
			  "���� ����ͦ������ ������� ��Ȧ�Φ �Φ, "
			  "� ��� ��Ԧ���� �ϧ���� � ������� �� ��֦. "
			  "��? �� ��? �� ��? �� ��? �� �� � �� �, "
			  "�ϧ ��������? - � �� ���� ������. "
			  "������� ����� � �� ����, �� ����� ���� - "
			  "����� ����� ���� � ����� Φ� �����.\n";

int main(void)
{
	set_system_clock();

	display_init();

	int x;

	while (1) {
		x = 0;

		for (int i = 0; i < sizeof(letters); i++) {
			x += display_putletter(letters[i]);
			for (volatile int j = 0; j < 500000; j++) ;
			if (x > 500 || letters[i] == '\n') {
				for (volatile int j = 0; j < 5000000; j++) ;
				x = 0;
				display_clear();
			}
		}

		for (volatile int j = 0; j < 5000000; j++);
		display_clear();
	}
	return 0;
}
