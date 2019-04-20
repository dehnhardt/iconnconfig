#include "audioroutingwidget.h"
#include "../sysex/getaudiopatchbayparm.h"
#include "../sysex/retsetaudioglobalparm.h"
#include "../sysex/retsetaudiopatchbayparm.h"
#include "ui_audioroutingwidget.h"

AudioRoutingWidget::AudioRoutingWidget(Device *device, QWidget *parent)
	: QWidget(parent), ui(new Ui::AudioRoutingWidget), m_pDevice(device) {
	ui->setupUi(this);
	this->m_iNumberOfAudioPorts =
		device->getAudioGlobalParm()->getNumberOfAudioPorts();
	loadData();
}

AudioRoutingWidget::~AudioRoutingWidget() { delete ui; }

void AudioRoutingWidget::loadData() {
	std::unique_ptr<GetAudioPatchbayParm> getAudioPatchdayParm =
		std::make_unique<GetAudioPatchbayParm>(m_pDevice);
	RoutingTableModel *routingTableModel = new RoutingTableModel();
	getAudioPatchdayParm->setDebug(true);
	for (unsigned int i = 0; i < m_iNumberOfAudioPorts; i++) {
		getAudioPatchdayParm->setPortId(i + 1);
		std::shared_ptr<RetSetAudioPatchbayParm> retSetAudioPatchbayParm =
			std::dynamic_pointer_cast<RetSetAudioPatchbayParm>(
				getAudioPatchdayParm->querySmart());
		routingTableModel->setAudioPatchbayParm(retSetAudioPatchbayParm);
	}
	ui->m_pTblRouting->setModel(routingTableModel);
}

void RoutingTableModel::setAudioPatchbayParm(
	const std::shared_ptr<RetSetAudioPatchbayParm> audioPatchbayParm) {
	if (m_iMaxInputChannel < audioPatchbayParm->getMaxInputChannel())
		m_iMaxInputChannel = audioPatchbayParm->getMaxInputChannel();
	if (m_iMaxOutputChannel < audioPatchbayParm->getMaxOutputChannel())
		m_iMaxOutputChannel = audioPatchbayParm->getMaxOutputChannel();
	m_iRoutingConfigs += audioPatchbayParm->getNumberOfPatchbayConfigBlocks();
	m_MapTableData.insert(audioPatchbayParm->getPortId(),
						  audioPatchbayParm->getAudioPatchbayConfiguration());
}

QVariant RoutingTableModel::data(__attribute__((unused))
								 const QModelIndex &index,
								 __attribute__((unused)) int role) const {
	return QVariant();
}
