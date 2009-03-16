class RenameTypeToSizeClassification < ActiveRecord::Migration
  def self.up
    rename_column :room_profiles, :type, :size_classification
  end

  def self.down
    rename_column :room_profiles, :size_classification, :type
  end
end
