#include "led.h"
#include "encoder.h"
#include "interrupt.h"
#include "uart.h"
#include "system.h"
#include "indicator.h"
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

	systick_init(9000, true); // 1 tick per ms

	indicator_init();

	int offset = 0, position;

	while (1) {
		position = 8;
		for (int i = 0; i < sizeof(letters); i++)
			position += indicator_putletter(letters[i], position - offset);

		offset = (offset + 1) % position;
		for (volatile int i = 0; i < 500000; i++);
	}
	return 0;
}
