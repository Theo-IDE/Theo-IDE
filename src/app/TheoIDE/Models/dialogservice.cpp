#include "dialogservice.hpp"

#include <qcontainerfwd.h>

#include "dialogbutton.hpp"
#include "messagedialogdto.hpp"

DialogService::DialogService(QObject* parent) : QObject(parent) {}
DialogService::~DialogService() {}

void DialogService::add(QSharedPointer<MessageDialogDTO> dto) {
  _dialogDTOs.append(dto);
  emit elementAdded();
}

std::optional<QSharedPointer<MessageDialogDTO>> DialogService::remove() {
  if (isEmpty()) {
    return std::nullopt;
  }
  const auto dto = _dialogDTOs.at(0);
  _dialogDTOs.removeFirst();
  return dto;
}

void DialogService::addReadPermissionDenied(const QString& fileName) {
  const QString title = tr("Permission denied");
  const QString text = tr("File can not be read");
  const QString detailedText = tr("Read permission denied.");
  const QString informativeText =
      tr("If you want to open this file, try to gain read access");
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(title, text, detailedText, informativeText));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

void DialogService::addFileDoesNotExist(const QString& fileName) {
  const QString title = tr("File missing");
  const QString text = tr("The file %1 does not exist").arg(fileName);
  const QString informativeText =
      tr("It seams you want to read a non existent file. This will not work. "
         "Please create it first.");
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(title, text, std::nullopt, informativeText));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

void DialogService::addUnsavedChangesInFile(
    const QString& fileName, std::function<void(void)> onSave,
    std::function<void(void)> onDiscard) {
  const QString title = tr("Unsaved changes");
  const QString text = tr("The document %1 has been modified").arg(fileName);
  const QString informativeText = tr("Do you want to save your changes?");
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(title, text, std::nullopt, informativeText));
  dto->setButtonWithCallback(DialogButton::Save, onSave);
  dto->setButtonWithCallback(DialogButton::Discard, onDiscard);
  add(dto);
}

void DialogService::addMaxReadFileSizeExceeded(const QString& fileName,
                                               const int maxFileSizeBytes) {
  const QString title = tr("Reading aborted");
  const QString text = tr("Reading of file %1 was aborted because the "
                          "maximum read size has been exceeded")
                           .arg(fileName);
  const QString detailedText =
      tr("The maximum read size is set to %1 bytes").arg(maxFileSizeBytes);
  const QString informativeText =
      tr("If you realy want to open such a large file, increase the maxiumum "
         "read size in the settings.");
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(title, text, detailedText, informativeText));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

bool DialogService::isEmpty() const { return _dialogDTOs.empty(); }

void DialogService::addNoScriptToCompile() {
  const QString title = tr("Nothing to compile");
  const QString text =
      tr("The compilation process was triggered, but there are no open tabs.");
  const QString informativeText =
      tr("Please open a temporary tab or read in a source file.");
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(title, text, std::nullopt, informativeText));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

void DialogService::addMainScriptIsEmpty() {
  const QString title = tr("Nothing to compile");
  const QString text = tr(
      "The compilation process was triggered, but the main script is empty.");
  const QString informativeText = tr("Please input some code.");
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(title, text, std::nullopt, informativeText));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

void DialogService::addNoMainScriptSelected() {
  const QString title = tr("Nothing to compile");
  const QString text = tr(
      "The compilation process was triggered, but no main script was selected");
  const QString informativeText = tr("Please select a main script");
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(title, text, std::nullopt, informativeText));
  dto->setButton(DialogButton::Ok);
  add(dto);
}
