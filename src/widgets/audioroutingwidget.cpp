#include "audioroutingwidget.h"
#include "../sysex/getaudiopatchbayparm.h"
#include "../sysex/getmixerportparm.h"
#include "../sysex/retsetaudiochannelname.h"
#include "../sysex/retsetaudioglobalparm.h"
#include "../sysex/retsetaudiopatchbayparm.h"
#include "../sysex/retsetaudioportparm.h"
#include "../sysex/retsetmixerinputparm.h"
#include "../sysex/retsetmixeroutputparm.h"
#include "portdisplayhelper.h"
#include "ui_audioroutingwidget.h"
#include "views/hierarchicalheaderview.h"

AudioRoutingWidget::AudioRoutingWidget(Device *device, QWidget *parent)
	: QWidget(parent), ui(new Ui::AudioRoutingWidget), m_pDevice(device) {
	ui->setupUi(this);
	this->m_iNumberOfAudioPorts =
		device->getAudioGlobalParm()->getNumberOfAudioPorts();

	const QString style = "QTableView { \
		 gridline-color: #aaaaa8;  \
		 padding: 0; \
		 font-size: 8pt; \
	} \
	QTableView::indicator{ \
		color: #b1b1b1; \
		border: 0; \
	} \
	QTableView::indicator:checked{\
		background-color: #a23232; \
		border-radius: 7px; \
		color: #010101; \
	} ";
	ui->m_pTblRouting->setStyleSheet(style);
	ui->m_pTblRouting->setFocusPolicy(Qt::NoFocus);
	m_pRoutingTableModel = new RoutingTableModel();
	loadHeaderStructure();
	loadTableData();
	connect(m_pRoutingTableModel, &RoutingTableModel::modelDataChanged, this,
			&AudioRoutingWidget::modelDataChanged);
}

AudioRoutingWidget::~AudioRoutingWidget() {
	delete ui;
	delete m_pRoutingTableModel;
}

void AudioRoutingWidget::loadHeaderStructure() {
	std::shared_ptr<AudioChannelStructure> audioChannelStructure =
		m_pDevice->getAudioChannelStructure();

	HierarchicalHeaderView *hvHorizontal =
		new HierarchicalHeaderView(Qt::Horizontal, this->ui->m_pTblRouting);
	hvHorizontal->setHighlightSections(true);
	hvHorizontal->setSectionsClickable(true);
	hvHorizontal->setDefaultSectionSize(20);
	this->ui->m_pTblRouting->setHorizontalHeader(hvHorizontal);

	HierarchicalHeaderView *hvVertical =
		new HierarchicalHeaderView(Qt::Vertical, this->ui->m_pTblRouting);
	hvVertical->setHighlightSections(true);
	hvVertical->setSectionsClickable(true);
	hvVertical->setDefaultSectionSize(20);
	this->ui->m_pTblRouting->setVerticalHeader(hvVertical);

	QStandardItem *inputRootItem = new QStandardItem("Sources");
	QStandardItem *outputRootItem = new QStandardItem("Sinks");
	// rotate
	inputRootItem->setData(1, Qt::UserRole);

	QList<QStandardItem *> inputPortList;
	QList<QStandardItem *> outputPortList;

	std::shared_ptr<RetSetMixerPortParm> retSetMixerPortParm =
		m_pDevice->getMixerPortParm();
	std::map<unsigned int, AudioPortMixerBlock> audioPortMixerBlocksMap =
		retSetMixerPortParm->getAudioPortMixerBlocks();

	for (auto audioPort : *audioChannelStructure) {
		AudioPortId audioPortId = audioPort.first;
		AudioDirectionChannels audioDirectionChannels = audioPort.second;
		AudioPortMixerBlock audioPortMixerBlock =
			audioPortMixerBlocksMap.at(audioPortId);

		for (auto audioDirections : audioDirectionChannels) {
			ChannelDirection channelDirection = audioDirections.first;
			auto audioChannels = audioDirections.second;
			std::shared_ptr<RetSetAudioPortParm> retSetAudioPortParm =
				m_pDevice->getAudioPort(audioPortId);
			QStandardItem *portItem =
				new QStandardItem(PortDisplayHelper::getAudioPortIcon(
									  retSetAudioPortParm->getAudioPortType()),
								  retSetAudioPortParm->getPortName().c_str());
			QList<QStandardItem *> channelsList;
			for (auto audioChannel : audioChannels) {
				AudioChannelId channelId = audioChannel.first;
				AudioPortChannelId channelIndex = channelIndex(
					audioPortId, AudioPortClass::PHYSICAL_PORT, channelId);
				std::shared_ptr<RetSetAudioChannelName> audioChannelName =
					audioChannel.second;
				QStandardItem *channelItem = new QStandardItem(
					audioChannelName->getChannelName().c_str());
				if (channelDirection == CD_INPUT) {
					m_vRows.push_back(channelIndex);
				} else if (channelDirection == CD_OUTPUT) {
					channelItem->setData(1, Qt::UserRole);
					m_vColumns.push_back(channelIndex);
				}
				channelsList.push_back(channelItem);
				portItem->appendColumn(channelsList);
				channelsList.clear();
			} // end for audioChannels
			if (channelDirection == CD_INPUT) {
				// push back audio port channels
				inputPortList.push_back(portItem);
				inputRootItem->appendColumn(inputPortList);
				inputPortList.clear();
				// and start with the mixer channels
				std::shared_ptr<AudioMixerOutputChannels>
					audioMixerOutputChannels =
						m_pDevice->getAudioMixerOutputChannels();
				if (audioPortMixerBlock.numberOfMixerOutputs > 0) {
					QStandardItem *portItem = new QStandardItem(
						PortDisplayHelper::getAudioPortIcon(
							retSetAudioPortParm->getAudioPortType()),
						QString(retSetAudioPortParm->getPortName().c_str()) +
							" Mix");

					for (unsigned int channelId = 1;
						 channelId <= audioPortMixerBlock.numberOfMixerOutputs;
						 ++channelId) {
						AudioPortChannelId channelIndex = channelIndex(
							audioPortId, AudioPortClass::MIXER_PORT, channelId);
						std::shared_ptr<RetSetMixerOutputParm> mixerOutputParm =
							audioMixerOutputChannels->at(channelIndex);
						QStandardItem *mixerChannelItem =
							new QStandardItem(QString::number(channelId));
						channelsList.push_back(mixerChannelItem);
						portItem->appendColumn(channelsList);
						channelsList.clear();
						m_vRows.push_back(channelIndex);
					}
					inputPortList.push_back(portItem);
					inputRootItem->appendColumn(inputPortList);
					inputPortList.clear();
				}
			}
			if (channelDirection == CD_OUTPUT) {
				// push back audio port channels
				outputPortList.push_back(portItem);
				outputRootItem->appendColumn(outputPortList);
				outputPortList.clear();
				// and start with the mixer channels
				if (audioPortMixerBlock.numberOfMixerInputs > 0) {
					QStandardItem *portItem = new QStandardItem(
						PortDisplayHelper::getAudioPortIcon(
							retSetAudioPortParm->getAudioPortType()),
						QString(retSetAudioPortParm->getPortName().c_str()) +
							" Mix");

					for (unsigned int channelId = 1;
						 channelId <= audioPortMixerBlock.numberOfMixerInputs;
						 ++channelId) {
						AudioPortChannelId channelIndex = channelIndex(
							audioPortId, AudioPortClass::MIXER_PORT, channelId);
						QStandardItem *mixerChannelItem =
							new QStandardItem(QString::number(channelId));
						mixerChannelItem->setData(1, Qt::UserRole);
						channelsList.push_back(mixerChannelItem);
						portItem->appendColumn(channelsList);
						channelsList.clear();
						m_vColumns.push_back(channelIndex);
					}
					outputPortList.push_back(portItem);
					outputRootItem->appendColumn(outputPortList);
					outputPortList.clear();
				}
			}
		} // end for audioDirectionChannels
	}

	m_pRoutingTableModel->m_pVerticalHeaderItemModel->setItem(0, 0,
															  inputRootItem);
	m_pRoutingTableModel->m_pHorizontalHeaderItemModel->setItem(0, 0,
																outputRootItem);

	m_pRoutingTableModel->setRows(m_vRows);
	m_pRoutingTableModel->setColumns(m_vColumns);
}

void AudioRoutingWidget::loadTableData() {
	std::unique_ptr<GetAudioPatchbayParm> getAudioPatchdayParm =
		std::make_unique<GetAudioPatchbayParm>(m_pDevice);
	getAudioPatchdayParm->setDebug(true);
	for (unsigned int i = 0; i < m_iNumberOfAudioPorts; i++) {
		getAudioPatchdayParm->setPortId(i + 1);
		std::shared_ptr<RetSetAudioPatchbayParm> retSetAudioPatchbayParm =
			std::dynamic_pointer_cast<RetSetAudioPatchbayParm>(
				getAudioPatchdayParm->querySmart());
		m_pRoutingTableModel->addAudioPatches(
			retSetAudioPatchbayParm->getAudioPatchbayConfiguration());
	}
	// Mixer Channels
	std::shared_ptr<MixerSink> mixerConnections = std::make_shared<MixerSink>();
	std::shared_ptr<AudioMixerInputChannels> audioMixerInputChannels =
		m_pDevice->getAudioMixerInputChannels();
	for (auto mixerInputParmPair : *audioMixerInputChannels) {
		MixerSource portSources;
		AudioPortChannelId sinkAudioPortChannelId = mixerInputParmPair.first;
		std::shared_ptr<RetSetMixerInputParm> mixerInput =
			mixerInputParmPair.second;
		unsigned int mixerSorcePort = mixerInput->getAudioSourcePortId();
		unsigned int mixerSourceChannel =
			mixerInput->getAudioSourceChannelNumber();
		AudioPortChannelId sourceAudioPortChannelId = channelIndex(
			mixerSorcePort, AudioPortClass::PHYSICAL_PORT, mixerSourceChannel);
		portSources[sourceAudioPortChannelId] = true;
		mixerConnections->insert(
			std::pair<AudioPortChannelId, std::map<AudioPortChannelId, bool>>(
				sinkAudioPortChannelId, portSources));
	}
	std::shared_ptr<AudioMixerOutputChannels> audioMixerOutputChannels =
		m_pDevice->getAudioMixerOutputChannels();
	for (auto mixerOutputParmPair : *audioMixerOutputChannels) {
		std::shared_ptr<RetSetMixerOutputParm> mixerOutput =
			mixerOutputParmPair.second;
		AudioPortId sinkPortId = mixerOutput->getPortId();
		unsigned int sinkChannelNumber =
			mixerOutput->getMixerSinkChannelNumber();
		AudioPortChannelId sinkAudioPortChannelId = channelIndex(
			sinkPortId, AudioPortClass::MIXER_PORT, sinkChannelNumber);
		std::vector<unsigned int> mixerSourceChannels =
			mixerOutput->getAudioSourceChannelNumbers();
		std::map<AudioPortChannelId, bool> portSources;
		for (unsigned int mixerSourceChannel : mixerSourceChannels) {
			AudioPortChannelId sourceAudioPortChannelId = channelIndex(
				sinkPortId, AudioPortClass::PHYSICAL_PORT, mixerSourceChannel);
			portSources[sinkAudioPortChannelId] = true;
			mixerConnections->insert(
				std::pair<AudioPortChannelId,
						  std::map<AudioPortChannelId, bool>>(
					sourceAudioPortChannelId, portSources));
		}
	}
	m_pRoutingTableModel->addAudioPatches(mixerConnections);

	ui->m_pTblRouting->setModel(m_pRoutingTableModel);
}

bool AudioRoutingWidget::modelDataChanged(QModelIndex index,
										  AudioPortChannelId source,
										  AudioPortChannelId sink) {
	AudioPortClass sourcePortClass = AudioPortClass((source / 100) % 10);
	AudioPortId sourcePortId = (source / 100 - sourcePortClass) / 10;
	AudioChannelId sourceChannelId = source % 100;
	AudioPortClass sinkPortClass = AudioPortClass((sink / 100) % 10);
	AudioPortId sinkPortId = (sink / 100 - sinkPortClass) / 10;
	AudioChannelId sinkChannelId = sink % 100;
	if ((sinkPortClass == AudioPortClass::PHYSICAL_PORT) &&
		(sourcePortClass == AudioPortClass::PHYSICAL_PORT)) {
		std::shared_ptr<
			std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>
			audioPatchbayConfiguration = std::make_shared<std::map<
				AudioPortChannelId, std::map<AudioPortChannelId, bool>>>();
		std::shared_ptr<AudioChannelStructure> ass =
			m_pDevice->getAudioChannelStructure();
		AudioDirectionChannels adc = ass->at(sinkPortId);

		for (auto channel : adc.at(ChannelDirection::CD_OUTPUT)) {
			std::map<AudioPortChannelId, bool> sourceChannels;
			AudioChannelId channelId = channel.first;
			AudioPortChannelId out = channelIndex(
				sinkPortId, AudioPortClass::PHYSICAL_PORT, channelId);
			bool connected = false;
			for (int row = 0;
				 row < m_pRoutingTableModel->rowCount(QModelIndex()); row++) {
				AudioPortChannelId in =
					m_pRoutingTableModel->getRowAudioPortChanneId(
						static_cast<unsigned long>(row));
				connected = m_pRoutingTableModel->getValue(out, in);
				if (connected) {
					sourceChannels[in] = true;
					break;
				}
			}
			if (!connected) {
				sourceChannels[0] = false;
			}
			audioPatchbayConfiguration->insert(
				std::pair<AudioPortChannelId,
						  std::map<AudioPortChannelId, bool>>(out,
															  sourceChannels));
		}
		std::unique_ptr<RetSetAudioPatchbayParm> retSetAudioPatchbayParm =
			std::make_unique<RetSetAudioPatchbayParm>(m_pDevice);
		retSetAudioPatchbayParm->setDebug(false);
		retSetAudioPatchbayParm->setPortId(sinkPortId);
		retSetAudioPatchbayParm->setCmdflags(SysExMessage::QUERY);
		retSetAudioPatchbayParm->setAudioPatchbayConfiguration(
			audioPatchbayConfiguration);
		if (retSetAudioPatchbayParm->execute() == 0) {
			loadTableData();
			return true;
		}
		return false;
	}
	return false;
}

RoutingTableModel::RoutingTableModel(QObject *parent)
	: QAbstractTableModel(parent) {
	m_pHorizontalHeaderItemModel = std::make_unique<QStandardItemModel>(parent);
	m_pVerticalHeaderItemModel = std::make_unique<QStandardItemModel>(parent);
}

void RoutingTableModel::setAudioPatchbayParm(
	const std::shared_ptr<RetSetAudioPatchbayParm> audioPatchbayParm) {
	addAudioPatches(audioPatchbayParm->getAudioPatchbayConfiguration());
	m_MapTableData.insert(
		audioPatchbayParm->getAudioPatchbayConfiguration()->begin(),
		audioPatchbayParm->getAudioPatchbayConfiguration()->end());
}

void RoutingTableModel::addAudioPatches(
	std::shared_ptr<
		std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>
		patches) {
	m_MapTableData.insert(patches->begin(), patches->end());
}

bool RoutingTableModel::getValue(AudioPortChannelId column,
								 AudioPortChannelId row) const {
	try {
		return m_MapTableData.at(column).at(row);
	} catch (__attribute__((unused)) const std::out_of_range &oor) {
		return false;
	}
}

QVariant RoutingTableModel::data(const QModelIndex &index, int role) const {
	AudioPortChannelId row = m_vRows[static_cast<unsigned long>(index.row())];
	AudioPortChannelId column =
		m_vColumns[static_cast<unsigned long>(index.column())];

	switch (role) {
	case Qt::CheckStateRole:
		return getValue(column, row) ? Qt::Checked : Qt::Unchecked;
	case Qt::BackgroundRole: {
		int r = 220;
		int g = 220;
		int b = 220;
		int add = 10;
		if ((row / 100) % 2 == 1) {
			r -= add;
			g -= add;
		}
		if ((column / 100) % 2 == 1) {
			r -= add;
			g -= add;
		}
		return QColor(r, g, b);
	}
	case Qt::ForegroundRole: {
		return QColor(200, 200, 255);
	}
	case HierarchicalHeaderView::HorizontalHeaderDataRole: {
		QVariant v;
		v.setValue(dynamic_cast<QObject *>(m_pHorizontalHeaderItemModel.get()));
		return v;
	}
	case HierarchicalHeaderView::VerticalHeaderDataRole: {
		QVariant v;
		v.setValue(dynamic_cast<QObject *>(m_pVerticalHeaderItemModel.get()));
		return v;
	}
	}
	return QVariant();
}

Qt::ItemFlags RoutingTableModel::flags(__attribute__((unused))
									   const QModelIndex &index) const {
	return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
}

bool RoutingTableModel::setData(const QModelIndex &index, const QVariant &value,
								int role) {
	if (role == Qt::CheckStateRole) {
		AudioPortChannelId row =
			m_vRows[static_cast<unsigned long>(index.row())];
		AudioPortChannelId column =
			m_vColumns[static_cast<unsigned long>(index.column())];
		if (!value.toBool()) {
			try {
				m_MapTableData.at(column).erase(row);
			} catch (__attribute__((unused)) const std::out_of_range &oor) {
				std::cout << "false" << std::endl;
				return false;
			}
		} else {
			eraseColumn(column);
			try {
				m_MapTableData.at(column).at(row) = true;
			} catch (__attribute__((unused)) const std::out_of_range &oor) {
				MixerSource s;
				try {
					s = m_MapTableData.at(column);
				} catch (__attribute__((unused)) const std::out_of_range &oor) {
					s = MixerSource();
				}
				s[row] = true;
				m_MapTableData[column] = s;
			}
		}
		if (modelDataChanged(index, row, column)) {
			emit dataChanged(
				createIndex(static_cast<int>(row), 0),
				createIndex(static_cast<int>(row),
							static_cast<int>(m_vColumns.size()) - 1),
				QVector<int>({role}));
			std::cout << "true" << std::endl;
			return true;
		}
	}
	std::cout << "false" << std::endl;
	return false;
}

void RoutingTableModel::eraseColumn(unsigned int column) {
	for (int i = 0; i < rowCount(QModelIndex()); i++) {
		try {
			m_MapTableData.at(column).erase(static_cast<unsigned int>(
				m_vRows[static_cast<unsigned int>(i)]));
		} catch (__attribute__((unused)) const std::out_of_range &oor) {
			// nothing to do if not already found
		}
	}
}
