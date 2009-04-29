class AlterRoomProfiles < ActiveRecord::Migration
  def self.up
    add_column :room_profiles, :google_map, :string, :limit => 4
  end

  def self.down
    remove_column :room_profiles, :google_map
  end
end
