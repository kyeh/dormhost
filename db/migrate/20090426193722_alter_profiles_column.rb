class AlterProfilesColumn < ActiveRecord::Migration
  def self.up
    change_column :profiles, :grad_year, :string, :limit => 4
    
  end

  def self.down
    change_column :profiles, :grad_year, :date
  end
end